#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "csapp.h"

#define PROTO_ERR -1


extern void *mzw_client_service(void *arg);
int debug_show_maze;
CLIENT_REGISTRY *client_registry;

void *mzw_client_service(void *arg){
	int fd = *((int*)arg);
	free(arg);
	pthread_detach(pthread_self());
	//DETATCHED
	creg_register(client_registry, fd);

	void** body = malloc(sizeof(void*));
	MZW_PACKET * in = malloc(sizeof(MZW_PACKET));
	MZW_PACKET * out = malloc(sizeof(MZW_PACKET));
	int loggedIn = 0;
	while(1){
		//IF ERR SHUTDOWN
		if(proto_recv_packet(fd, in, body) == PROTO_ERR){
			free(in);
			free(out);
			creg_unregister(client_registry, fd);
			shutdown(fd,SHUT_RD);
			return NULL;
		}

		if(!loggedIn){
			if(in->type != MZW_LOGIN_PKT){
				continue;
			}
			if((player = player_login(in->param1)) != NULL){
				loggedin = 1;
				out->type = MZW_READY_PKT;
				player_reset(fd, in->param1, (char*)(*body));
				proto_send_packet(fd,out,NULL);
			}else{
				out->type = MZW_INUSE_PKT;
				proto_send_packet(fd,out,NULL);
			}
			continue;

		}
		if(in->type == MZW_LOGIN_PKT){
			continue;
		}



	}
}
