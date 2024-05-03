#ifndef		__PACKET_H
#define		__PACKET_H

#include <linux/if_packet.h>

enum {
	BUFSIZE		= 65535
};

enum {
	ICMP_ECHO_REPLY = 0x0,
	ICMP_ECHO_REQ = 0x8
};

/* 
 * Header for icmp echo messages. 
 * Different messages have differnet headers
 */
typedef struct icmphdr {
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
	uint16_t	id;
	uint16_t	seq;
} icmphdr_t;

typedef struct packetbuf {
	struct sockaddr_ll		sockaddr;		/* link layer address of the packet */
	uint8_t					*rawp;			/* pointer to buffer pointing to the raw packet */
	ssize_t					len;			/* length of data in the buffer */
	struct iphdr			*iphdr;			/* IPv4 Header */
	struct icmphdr			*icmphdr;		/* IcmpV4 Header */
	uint8_t					*payloadp;		/* pointer to payload */
	ssize_t					payload_len;	/* length of payload */
} packetbuf_t;


void receive_packet(int, packetbuf_t *);
void send_packet(int,packetbuf_t *);

void craft_packet(packetbuf_t *, packetbuf_t *);

void init_packetbuf(packetbuf_t *);
void free_packetbuf(packetbuf_t *);

#endif
