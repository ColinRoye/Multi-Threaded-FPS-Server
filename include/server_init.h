#ifndef SERVER_INIT
#define SERVER_INIT
extern char *template_file;
extern char *port;
void getArgs(int argc, char*argv[]);
char** process_template(char* file);
#endif
