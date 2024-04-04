#ifndef BASIC_HTTP_SERVER_H
#define BASIC_HTTP_SERVER_H

#include "../../lib/http-lib/httplib.h"

#define DOCUMENT_ROOT                                                          \
  "../src/root/" // TODO: make this cleaner? Issue:
              // https://git.fh-muenster.de/pse2024/PG5_1/pse-2024/-/issues/4
#define HTTP_VERSION "HTTP/1.0"

struct basic_request {
  string *method;
  string *ressource;
  string *version;
} typedef basic_request;

struct basic_response {
  string *version;
  string *status_code;
  string *status_message;
  string *content_type;
  string *content_length;
  string *body;
} typedef basic_response;

/// @brief Start the basic http server.
/// @param request The request.
/// @return The response.
string *basic_http_server(string *request);

#endif
