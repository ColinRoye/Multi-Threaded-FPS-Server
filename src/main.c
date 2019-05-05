#include <stdlib.h>
#include <getopt.h>
#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "server_init.h"

#include "csapp.h"


extern char *template_file;
extern char *port;

static void terminate(int status);
static char *default_maze[] = {
  "******************************",
  "***** %%%%%%%%% &&&&&&&&&&& **",
  "***** %%%%%%%%%        $$$$  *",
  "*           $$$$$$ $$$$$$$$$ *",
  "*##########                  *",
  "*########## @@@@@@@@@@@@@@@@@*",
  "*           @@@@@@@@@@@@@@@@@*",
  "******************************",
  NULL
};

CLIENT_REGISTRY *client_registry;

int main(int argc, char* argv[]){
    // Option processing should be performed here.
    // Option '-p <port>' is required in order to specify the port number
    // on which the server should listen.
    getArgs(argc, argv);
    debug("port: %s", port);
    debug("template_file: %s", template_file);
    // Perform required initializations of the client_registry,
    client_registry = creg_init();
    // maze, and player modules.
    if(template_file != NULL){
          maze_init(process_template(template_file));
    }else{
          maze_init(default_maze);
   }
    player_init();

    debug_show_maze = 1;  // Show the maze after each packet.

    // TODO: Set up the server socket and enter a loop to accept connections
    // on this socket.  For each connection, a thread should be started to
    // run function mzw_client_service().  In addition, you should install
    // a SIGHUP handler, so that receipt of SIGHUP will perform a clean
    // shutdown of the server.'
    int listenfd, *connfdp;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;
    listenfd = Open_listenfd(port);
    debug("listenfd: %d", listenfd);

    while(1){
          clientlen = sizeof(struct sockaddr_storage);
          connfdp = Malloc(sizeof(int));
          *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
          pthread_create(&tid, NULL, mzw_client_service, connfdp);
   }



    terminate(EXIT_FAILURE);
}

/*
 * Function called to cleanly shut down the server.
 */
void terminate(int status) {
    // Shutdown all client connections.
    // This will trigger the eventual termination of service threads.
    creg_shutdown_all(client_registry);

    debug("Waiting for service threads to terminate...");
    creg_wait_for_empty(client_registry);
    debug("All service threads terminated.");

    // Finalize modules.
    creg_fini(client_registry);
    player_fini();
    maze_fini();

    debug("MazeWar server terminating");
    exit(status);
}
