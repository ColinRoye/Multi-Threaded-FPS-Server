
#include <stdlib.h>
#include <getopt.h>
#include "client_registry.h"
#include "maze.h"
#include "player.h"
#include "debug.h"
#include "server.h"
#include "csapp.h"

char* port;
char* template_file;

void getArgs(int argc, char *argv[]){
      int i;
      while((i = getopt(argc, argv, ":p:")) != -1){
          switch(i){
            case 'p': port = Malloc(sizeof(optarg));
                  memcpy(port, optarg, strlen(optarg)+1);
                  break;
            case 't': template_file = Malloc(sizeof(optarg));
                  memcpy(template_file, optarg, strlen(optarg)+1);
                  break;
            default: fprintf(stderr, "ERROR: invalid option\n");
                  exit(1);
                  break;
          }
    }


}
char** process_template(char* file){
      //TODO
      return NULL;
}
