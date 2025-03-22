#ifndef server_h
#define server_h

#include <netinet/in.h>
#include <stdlib.h>

typedef struct Server {
  int server_fd;
  struct sockaddr_in address;
} server_t;

void init_server(server_t *server, int port);
void listen_and_serve(server_t *server, int client_socket,
                      void *(handler)(int));
void deinit_server(server_t *server);

#endif
