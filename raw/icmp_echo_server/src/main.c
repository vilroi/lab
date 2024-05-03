#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

#include "packet.h"
#include "error.h"
#include "utils.h"

int 
main(void)
{
	int			sockfd;
	packetbuf_t	recvbuf, sendbuf;

	if ((sockfd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL))) == -1)
		ERR_EXIT("socket failed");

	init_packetbuf(&recvbuf);
	init_packetbuf(&sendbuf);

	while (1) {
		receive_packet(sockfd, &recvbuf);
		craft_packet(&sendbuf, &recvbuf);
		send_packet(sockfd, &sendbuf);

	}
	exit(EXIT_SUCCESS);
}


