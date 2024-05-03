#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <err.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#include "error.h"
#include "packet.h"
#include "utils.h"

char  icmp_data[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 
					  0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 
					  0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 
					  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};


static void craft_icmp(packetbuf_t *, packetbuf_t *);
static void craft_ip(packetbuf_t *, packetbuf_t *);
static void craft_payload(uint8_t *);

static void dump_sockaddr_ll(struct sockaddr_ll *);


void
receive_packet(int sockfd, packetbuf_t *packetbuf)
{
	socklen_t	socklen = sizeof(struct sockaddr_ll);
	ssize_t		recvlen;

	while (1) {
		if ((recvlen = recvfrom(sockfd, packetbuf->rawp, BUFSIZE, 0,
							(struct sockaddr *) &packetbuf->sockaddr, &socklen)) == -1)
			ERR_EXIT("recvfrom failed");

		if (packetbuf->iphdr->protocol != IPPROTO_ICMP)
			continue;

		if (packetbuf->icmphdr->type != ICMP_ECHO_REQ)
			continue;

		packetbuf->len = recvlen;
		packetbuf->payload_len = recvlen - (sizeof(struct iphdr) + sizeof(struct icmphdr));

		break;
	}
}

void 
craft_packet(packetbuf_t *new, packetbuf_t *old)
{
	memcpy(&new->sockaddr, &old->sockaddr, sizeof(struct sockaddr_ll));
	
	new->len = old->len;
	new->payload_len = old->payload_len;
	//memcpy(new->payloadp, old->payloadp, new->payload_len);
	craft_payload(new->payloadp);

	craft_icmp(new, old);
	craft_ip(new, old);
}

static void 
craft_icmp(packetbuf_t *new, packetbuf_t *old)
{
	/* 
	 * technically setting the type and code shouldn't be necessary
	 * as these buffers have been calloc'd, and they are only used
	 * for icmp echo replies.
	 */
	new->icmphdr->type = 0;
	new->icmphdr->code = 0;
	new->icmphdr->checksum = 0;
	new->icmphdr->id = old->icmphdr->id;
	new->icmphdr->seq = old->icmphdr->seq;

	new->icmphdr->checksum = calc_checksum((uint8_t *) new->icmphdr, sizeof(struct icmphdr) + old->payload_len);
}

static void 
craft_ip(packetbuf_t *new, packetbuf_t *old)
{
	new->iphdr->version = 4;
	new->iphdr->ihl = 5;
	new->iphdr->tos = old->iphdr->tos;
	new->iphdr->tot_len = htons(old->len);
	new->iphdr->id = genid();
	new->iphdr->frag_off = 0;
	new->iphdr->ttl = 64;
	new->iphdr->protocol = IPPROTO_ICMP;
	new->iphdr->check = 0;
	new->iphdr->saddr = old->iphdr->daddr;
	new->iphdr->daddr = old->iphdr->saddr;

	new->iphdr->check = calc_checksum((uint8_t *) new->iphdr, sizeof(struct iphdr));
}

static void
craft_payload(uint8_t *payload)
{
	struct timeval	*tv;

	tv = (struct timeval *) payload;

	if (gettimeofday(tv, NULL) == -1)
		ERR_EXIT("gettimeofday failed");

	payload += sizeof(struct timeval);
	memcpy(payload, icmp_data, sizeof(icmp_data));
}

void 
send_packet(int sockfd, packetbuf_t *sendbuf)
{
	socklen_t	socklen = sizeof(struct sockaddr_ll);
	ssize_t		nsent;

	if ((nsent = sendto(sockfd, sendbuf->rawp, sendbuf->len, 0,
				(struct sockaddr *) &sendbuf->sockaddr, socklen)) == -1)
		ERR_EXIT("sendto failed");
}

void
init_packetbuf(packetbuf_t *packetbuf)
{
	if (packetbuf == NULL)
		ERROR_EXIT("null pointer passed as argument");

	if ((packetbuf->rawp = calloc(BUFSIZE, sizeof(uint8_t))) == NULL)
		ERR_EXIT("calloc failed");

	packetbuf->iphdr = (struct iphdr *) packetbuf->rawp;
	packetbuf->icmphdr = (struct icmphdr *) (packetbuf->iphdr + 1);
	packetbuf->payloadp = (uint8_t *) (packetbuf->icmphdr + 1);
}

void 
free_packetbuf(packetbuf_t *packetbuf)
{
	if (packetbuf == NULL)
		return;

	if (packetbuf->rawp == NULL)
		return;

	free(packetbuf->rawp);
}

static void
dump_sockaddr_ll(struct sockaddr_ll *sockaddr)
{
	printf("addr: %x:%x:%x:%x:%x:%x\n",
			sockaddr->sll_addr[0],
			sockaddr->sll_addr[1],
			sockaddr->sll_addr[2],
			sockaddr->sll_addr[3],
			sockaddr->sll_addr[4],
			sockaddr->sll_addr[5]);
}
