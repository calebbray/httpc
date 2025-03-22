
#include "include/http.h"
#include "include/server.h"
#include <stdio.h>

#define PORT 8080

int main() {
  int client_socket;
  server_t server;
  init_server(&server, PORT);
  printf("HTTP Server running on port %d...\n", PORT);
  listen_and_serve(&server, client_socket, (void *)handle_client);

  deinit_server(&server);
  return 0;
}
