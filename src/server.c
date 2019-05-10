#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>

#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "protocol.h"
#include "client_registry.h"
#include "maze.h"
#include "debug.h"
#include "server.h"
#include "server_init.h"

#include "debug.h"
#include "csapp.h"

#define PROTO_ERR -1
extern void debugPacket(MZW_PACKET* p);
extern void *mzw_client_service(void *arg);
int debug_show_maze;
CLIENT_REGISTRY *client_registry;

int hit;
void laser_handler(){
	hit = 1;
}

void *mzw_client_service(void *arg){
	int fd = *((int*)arg);
	//Signal(SIGUSR1, laser_handler);

	free(arg);
	pthread_detach(pthread_self());
	//DETATCHED
	creg_register(client_registry, fd);
	pthread_detach(pthread_self());

	void* player;
	void* payload;
	MZW_PACKET * in = malloc(sizeof(MZW_PACKET));
	//MZW_PACKET out;// = malloc(sizeof(MZW_PACKET));
	int loggedIn = 0;
	while(1){
		if(loggedIn){
			player_check_for_laser_hit(player);
			hit = 0;
		}
		//IF ERR SHUTDOWN
		if(proto_recv_packet(fd, in, &payload) < 0 && errno != EINTR){
			free(in);
			player_logout(player);
			creg_unregister(client_registry, fd);
			shutdown(fd,SHUT_RD);
			return NULL;
		}
		if(!loggedIn){
			if(in->type != MZW_LOGIN_PKT){
				continue;
			}
			if((player = player_login(fd, in->param1, (char*)payload)) != NULL){
				loggedIn = 1;
				MZW_PACKET out = {MZW_READY_PKT, 0, 0, 0, 0, 0, 0};
				proto_send_packet(fd,&out,NULL);
				player_reset(player);

			}else{
				MZW_PACKET out = {MZW_INUSE_PKT, 0, 0, 0, 0, 0, 0};
				proto_send_packet(fd,&out,NULL);
			}
			continue;
		}
		if(in->type == MZW_LOGIN_PKT){
			continue;
		}
		switch(in->type){
			case MZW_MOVE_PKT: debug("MZW_MOVE_PKT:");
				player_move(player, in->param1);
				break;
			case MZW_TURN_PKT: debug("MZW_TURN_PKT:");
				player_rotate(player, in->param1);
				break;
			case MZW_FIRE_PKT: debug("MZW_FIRE_PKT:");
				player_fire_laser(player);
				break;
			case MZW_REFRESH_PKT: debug("MZW_REFRESH_PKT:");
				player_invalidate_view(player);
				player_update_view(player);
				break;
			case MZW_SEND_PKT: debug("MZW_CHAT_PKT:");
				player_send_chat(player, payload, in->size);
				break;
			default:
			debug("type not recognized %d", in->type);
		}

	}
}
void debugPacket(MZW_PACKET* p){
	debug("type %d", p->type);
	debug("param1 %d", p->param1);
	debug("param3 %d", p->param2);
	debug("param2 %d", p->param3);
	debug("size %d", p->size);

}
