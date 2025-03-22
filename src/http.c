#include "../include/http.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  const char *name;
  http_method_t method;
} method_mapping_t;

const method_mapping_t method_map[] = {
    {"GET", HTTP_GET},         {"POST", HTTP_POST},   {"HEAD", HTTP_HEAD},
    {"OPTIONS", HTTP_OPTIONS}, {"PATCH", HTTP_PATCH}, {"DELETE", HTTP_DELETE},
    {"PUT", HTTP_PUT},         {NULL, HTTP_INVALID},
};

static http_method_t get_http_method(const char *method) {
  for (int i = 0; method_map[i].name != NULL; i++) {
    if (strcmp(method_map[i].name, method) == 0) {
      return method_map[i].method;
    }
  }

  return HTTP_INVALID;
}

void init_response(http_response_t *res, http_status_code_t status) {
  snprintf(res->status, sizeof(res->status), "HTTP/1.1 %d OK", status);
  add_header(res, "Connection", "keep-alive");
}

void add_header(http_response_t *res, const char *header_name,
                const char *header_value) {
  char header[256];
  snprintf(header, sizeof(header), "%s: %s\r\n", header_name, header_value);
  strcat(res->headers, header);
}

void set_body(http_response_t *res, const char *body) {
  int len = strlen(body);
  memcpy(res->body, body, len);
  res->body_length = len;
}

void send_response(http_response_t *res, int client_socket) {
  char response_header[BUFFER_SIZE];
  snprintf(response_header, sizeof(response_header),
           "%s\r\n"
           "%s"
           "Content-Length: %zu\r\n"
           "\r\n",
           res->status, res->headers, res->body_length);
  send(client_socket, response_header, strlen(response_header), 0);
  if (res->body_length > 0) {
    send(client_socket, res->body, res->body_length, 0);
  }
}

void handle_client(int client_socket) {
  char buffer[BUFFER_SIZE];
  int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
  if (bytes_read < 0) {
    perror("read failed");
    close(client_socket);
    return;
  }

  buffer[bytes_read] = '\0';
  printf("received request:\n%s\n", buffer);

  char method[8], path[256];
  int scanned = sscanf(buffer, "%7s %255s", method, path);
  if (scanned != 2) {
    close(client_socket);
    return;
  }

  // Check for Host header (basic validation)
  if (strstr(buffer, "Host:") == NULL) {
    const char *bad_request_response = "HTTP/1.1 400 Bad Request\r\n"
                                       "Content-Type: text/plain\r\n"
                                       "Content-Length: 16\r\n"
                                       "Connection: close\r\n"
                                       "\r\n"
                                       "400 Bad Request";
    send(client_socket, bad_request_response, strlen(bad_request_response), 0);
    close(client_socket);
    return;
  }

  switch (get_http_method(method)) {
  case HTTP_GET: {

    http_response_t res;
    init_response(&res, HTTP_STATUS_OK);

    add_header(&res, "Connection", "close");
    add_header(&res, "Content-Type", "text/plain");

    set_body(&res, "Good Job");

    send_response(&res, client_socket);
    close(client_socket);
  }
  default: {
    const char *bad_request_response = "HTTP/1.1 400 Bad Request\r\n"
                                       "Content-Type: text/plain\r\n"
                                       "Content-Length: 16\r\n"
                                       "Connection: close\r\n"
                                       "\r\n"
                                       "400 Bad Request";
    send(client_socket, bad_request_response, strlen(bad_request_response), 0);
    close(client_socket);
  }
  }

  return;
}
