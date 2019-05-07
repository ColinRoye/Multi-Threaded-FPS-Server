
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "client_registry.h"
#include "debug.h"
#include <sys/socket.h>
#define MAX_CLIENTS 500

typedef struct client_registry{
	pthread_mutex_t mutex;
	int clients;
	int fds[MAX_CLIENTS];
} CLIENT_REGISTRY;


CLIENT_REGISTRY *creg_init(){
	CLIENT_REGISTRY* cr = malloc(sizeof(CLIENT_REGISTRY));
	pthread_mutex_init(&cr->mutex, NULL);
	for(int i = 0; i < MAX_CLIENTS; i++){
		cr->fds[i] = -1;
	}
	return cr;
}

void creg_fini(CLIENT_REGISTRY *cr){
	pthread_mutex_destroy(&cr->mutex);
	free(cr);
}

void creg_register(CLIENT_REGISTRY *cr, int fd){
	debug("reg start");

	pthread_mutex_lock(&cr->mutex);
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(cr->fds[i] == -1){
			cr->fds[i] =  fd;
			cr->clients++;
			pthread_mutex_unlock(&cr->mutex);
			debug("reg fin");
			return;

		}
	}
	debug("max clients reached");
	pthread_mutex_unlock(&cr->mutex);

}

void creg_unregister(CLIENT_REGISTRY *cr, int fd){
	pthread_mutex_lock(&cr->mutex);
	cr->clients--;
	pthread_mutex_unlock(&cr->mutex);
}

void creg_wait_for_empty(CLIENT_REGISTRY *cr){
	while(1){
		pthread_mutex_lock(&cr->mutex);
		if(cr->clients == 0){
			pthread_mutex_unlock(&cr->mutex);
			return;
		}
		pthread_mutex_unlock(&cr->mutex);
	}
}

void creg_shutdown_all(CLIENT_REGISTRY *cr){
	pthread_mutex_lock(&cr->mutex);
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(cr->fds[i] != -1){
			shutdown(cr->fds[i], SHUT_RD);
		}
	}
	pthread_mutex_unlock(&cr->mutex);
}
