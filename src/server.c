
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "csapp.h"

//
// void *mzw_client_service(void *arg){
// 	int fd = *((int)arg);
// 	free(arg);
// 	pthread_detach(pthread_self());
// 	creg_register(cr, fd);
// }