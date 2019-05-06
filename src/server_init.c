
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

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
      int port_flag = 1;
      while((i = getopt(argc, argv, ":p:t:")) != -1){
          switch(i){
            case 'p':
                  port = malloc(strlen(optarg));
                  memcpy(port, optarg, strlen(optarg));
                  port_flag = 0;
                  break;
            case 't':
                  template_file = malloc(strlen(optarg));
                  memcpy(template_file, optarg, strlen(optarg));
                  break;
            default: fprintf(stderr, "ERROR: invalid option\n");
                  exit(1);
                  break;
          }
    }
   if(port_flag){

       exit(1);

   }


}
char** process_template(char* tf){
      FILE *file;
      file = fopen(tf, "r");
      fseek(file, 0, SEEK_SET);
      int row = 0;
      int col = 0;
      char** in;
      char c_prev;
      char c;
      while((c = fgetc(file)) != EOF){
            if(c == '\n'){
                  row++;
            }
            if(row == 0) {
                  col++;
            }
            c_prev = c;
      }
      if(c_prev != '\n'){
            row++;
      }

      fseek(file, 0, SEEK_SET);
      in = malloc(sizeof(char*) * (row+1));
      for(int i = 0; i < row; i++){
            in[i] = malloc(sizeof(char) * col);
            for(int j = 0; j < col+1; j++){
                  if((c = fgetc(file)) != '\n' && c != -1){
                        in[i][j] = c;
                  }
            }
      }
      in[row] = NULL;
      return in;
}
