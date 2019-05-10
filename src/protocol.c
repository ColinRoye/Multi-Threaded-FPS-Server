#include <stdlib.h>
#include <getopt.h>
#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "server_init.h"

#include "csapp.h"
extern int proto_send_packet(int fd, MZW_PACKET *pkt, void *data);
extern int proto_recv_packet(int fd, MZW_PACKET *pkt, void **data);





int rio_writen_safe(int fd, void *usrbuf, size_t n){
    if (rio_writen(fd, usrbuf, n) != n)
	return 0;
    return 1;
}
int proto_send_packet(int fd, MZW_PACKET *pkt, void *data){
	size_t packetLen = sizeof(MZW_PACKET);
	uint32_t len = pkt->size;
	pkt->timestamp_sec = 0;
	pkt->timestamp_nsec = 0;
	pkt->size = htons(pkt->size);
	pkt->timestamp_sec = htonl(pkt->timestamp_sec);
	pkt->timestamp_nsec = htonl(pkt->timestamp_nsec);

	if (write(fd, pkt, packetLen) != packetLen){
		return -1;
	}
	if (len != 0 && write(fd, data, len) != len){
		return -1;
	}
      return 0;

}
int proto_recv_packet(int fd, MZW_PACKET *pkt, void **datap){
	int test;

	if ((test = read(fd, pkt, sizeof(MZW_PACKET))) != sizeof(MZW_PACKET)){
		debug("error, actually read in %d expected %d", test, (int)sizeof(MZW_PACKET));
		return -1;
	}

	pkt->size = ntohs(pkt->size);
	pkt->timestamp_sec = ntohl(pkt->timestamp_sec);
	pkt->timestamp_nsec = ntohl(pkt->timestamp_nsec);


	void* x = calloc(sizeof(char), pkt->size);

	if (pkt->size > 0 && (test = read(fd, x, pkt->size)) != pkt->size){
		debug("err2 actually read in %d", test);
		return -1;
	}
	if(pkt->type == 0){
		return -1;
	}

      *datap = x;
      return 0;

}







//
//
//
//
//
//
//
// #include <stdlib.h>
// #include <getopt.h>
// #include "client_registry.h"
// #include "maze.h"
// #include "player.h"
// #include "debug.h"
// #include "server.h"
// #include "server_init.h"
//
// #include "csapp.h"
// extern int proto_send_packet(int fd, MZW_PACKET *pkt, void *data);
// extern int proto_recv_packet(int fd, MZW_PACKET *pkt, void **data);
//
// void network_byte_order(MZW_PACKET* pkt){
// 	//pkt->type = htonl(pkt->type);
// 	// pkt->param1 = htonl(pkt->param1);
// 	// pkt->param2 = htonl(pkt->param2);
// 	// pkt->param3 = htonl(pkt->param3);
// 	pkt->size = htonl(pkt->size);

// }
// void host_byte_order(MZW_PACKET* pkt){
// 	//0pkt->type = ntohl(pkt->type);
// 	// pkt->param1 = ntohl(pkt->param1);
// 	// pkt->param2 = ntohl(pkt->param2);
// 	// pkt->param3 = ntohl(pkt->param3);
// 	pkt->size = ntohl(pkt->size);
// 	pkt->timestamp_sec = ntohl(pkt->timestamp_sec);
// 	pkt->timestamp_nsec = ntohl(pkt->timestamp_nsec);
// }
//
// void setTimeStamp(MZW_PACKET* pkt){
//
// }
// size_t rio_readn_safe(int fd, void *ptr, size_t nbytes){
//     ssize_t n;
//
//     if ((n = rio_readn(fd, ptr, nbytes)) < 0)
// 	return 0;
//     return n;
// }
// int rio_writen_safe(int fd, void *usrbuf, size_t n){
//     if (rio_writen(fd, usrbuf, n) != n)
// 	return 0;
//     return 1;
// }
// int proto_send_packet(int fd, MZW_PACKET *pkt, void *data){
// 	//setTimeStamp(pkt);
// 	debug("sent %d", pkt->size);
//
// 	network_byte_order(pkt);
// 	if(!rio_writen_safe(fd, pkt, sizeof(MZW_PACKET))){
// 		return -1;
// 	}
// 	if(!rio_writen_safe(fd, data, pkt->size)){
// 		return -1;
// 	}
// 	return 0;
// }
// int proto_recv_packet(int fd, MZW_PACKET *pkt, void **datap){
// 	int n;
// 	if(!(n = rio_readn_safe(fd, pkt, sizeof(MZW_PACKET)))){
// 		return 1;
// 	}
// 	debug("recv %d", pkt->size);
//
// 	host_byte_order(pkt);
// 	debug("recv %d", pkt->size);
//
// 	if(pkt->size > 0){
// 		*datap = Malloc(pkt->size);
// 		if(!(n = rio_readn_safe(fd, *datap, pkt->size))){
// 			free(*datap);
// 			return 1;
// 		}
// 	}
// 	debug("recv %d", pkt->size);
//
// 	return 0;
//
// }