#include <stdlib.h>
#include <errno.h>
#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "server_init.h"

#include "csapp.h"
extern int proto_send_packet(int fd, MZW_PACKET *pkt, void *data);
extern int proto_recv_packet(int fd, MZW_PACKET *pkt, void **data);






int proto_send_packet(int fd, MZW_PACKET *pkt, void *data){
	size_t packetLen = sizeof(MZW_PACKET);
	uint32_t len = pkt->size;
	pkt->timestamp_sec = 0;
	pkt->timestamp_nsec = 0;
	pkt->size = htons(pkt->size);
	pkt->timestamp_sec = htonl(pkt->timestamp_sec);
	pkt->timestamp_nsec = htonl(pkt->timestamp_nsec);

	if (rio_writen(fd, pkt, packetLen) != packetLen){
		return -1;
	}
	if (len != 0 && rio_writen(fd, data, len) != len){
		return -1;
	}
      return 0;

}
int proto_recv_packet(int fd, MZW_PACKET *pkt, void **datap){

	int n;
	if((n = rio_readn(fd, pkt, sizeof(MZW_PACKET))) != sizeof(MZW_PACKET)){

			return -1;

	}

	pkt->size = ntohs(pkt->size);
	pkt->timestamp_sec = ntohl(pkt->timestamp_sec);
	pkt->timestamp_nsec = ntohl(pkt->timestamp_nsec);


	void* x = calloc(sizeof(char), pkt->size);

	if (pkt->size > 0 && (n = rio_readn(fd, x, pkt->size)) != pkt->size){

			return -1;

	}
	if(pkt->type == 0){
		return -1;
	}

      *datap = x;
      return 0;

}





