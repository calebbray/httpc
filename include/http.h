
#ifndef http_h
#define http_h

#include "server.h"

#define BUFFER_SIZE 4096

typedef enum HTTP_METHOD {
  HTTP_GET,
  HTTP_POST,
  HTTP_HEAD,
  HTTP_OPTIONS,
  HTTP_PATCH,
  HTTP_DELETE,
  HTTP_PUT,
  HTTP_INVALID,
} http_method_t;

typedef enum HTTP_RESPONSE_STATUS {
  HTTP_STATUS_OK = 200,
  HTTP_STATUS_BAD_REQUEST = 400,
} http_status_code_t;

typedef struct HttpResponse {
  size_t body_length;
  char status[32];
  char headers[1024];
  char body[BUFFER_SIZE];
} http_response_t;

static http_method_t get_http_method(const char *method);
void add_header(http_response_t *res, const char *header_name,
                const char *header_value);
void init_response(http_response_t *res, http_status_code_t status);
void set_body(http_response_t *res, const char *body);
void send_response(http_response_t *res, int client_socket);
void handle_client(int client_socket);

#endif
