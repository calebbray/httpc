#include <stdio.h>
#include <stdlib.h>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/server.h"

void init_server(server_t *server, int port) {
  int server_fd;
  struct sockaddr_in address;
  int addlen = sizeof(address);

  // Create a socket
  server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server->server_fd == 0) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // Configure Address
  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(port);

  // Bind socket to port
  int bind_status = bind(server->server_fd, (struct sockaddr *)&server->address,
                         sizeof(address));
  if (bind_status < 0) {
    perror("bind failed");
    close(server->server_fd);
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(server->server_fd, 3) < 0) {
    perror("listen failed");
    close(server->server_fd);
    exit(EXIT_FAILURE);
  }
}

void listen_and_serve(server_t *server, int client_socket,
                      void *(handler)(int)) {
  while (1) {
    // Accept client connections
    client_socket =
        accept(server->server_fd, (struct sockaddr *)&server->address,
               (socklen_t *)&server->address);
    if (client_socket < 0) {
      perror("accept failed");
      continue;
    }

    handler(client_socket);
    close(client_socket);
  }
}

void deinit_server(server_t *server) { close(server->server_fd); }
