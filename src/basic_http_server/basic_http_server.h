#ifndef BASIC_HTTP_SERVER_H
#define BASIC_HTTP_SERVER_H

#include "../../lib/http-lib/httplib.h"

#define SERVER_SIGNATURE "LLDM/0.1 Basic HTTP Server"
#define DOCUMENT_ROOT "src/root/" // TODO: make this cleaner? Issue:
// https://git.fh-muenster.de/pse2024/PG5_1/pse-2024/-/issues/4
// Set to `../src/root/` when running the program with jetbrains (clion)
// Set to `src/root/` when running the program manually

#define HTTP_VERSION_1_0 "HTTP/1.0"
#define HTTP_VERSION_1_1 "HTTP/1.1"
#define CONTENT_TYPE_HTML "text/html"
#define HTTP_LINE_BREAK "\r\n"

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_SERVER_ERROR 500

#define STATUS_MESSAGE_OK "OK"
#define STATUS_MESSAGE_BAD_REQUEST "Bad Request"
#define STATUS_MESSAGE_FORBIDDEN "Forbidden"
#define STATUS_MESSAGE_NOT_FOUND "Not Found"
#define STATUS_MESSAGE_INTERNAL_SERVER_ERROR "Internal Server Error"
#define STATUS_MESSAGE_UNKNOWN "Unknown"

#define CONTENT_LENGTH_HEADER "Content-Length: "
#define CONTENT_TYPE_HEADER "Content-Type: "
#define SERVER_HEADER "Server: "

struct basic_request {
  string *method;
  string *resource;
  string *version;
} typedef basic_request;

struct basic_response {
  string *version;
  string *status_code;
  string *status_message;
  string *content_type;
  string *content_length;
  string *server;
  string *body;
} typedef basic_response;

/// @breif Starts a basic HTTP server
/// @param request Raw HTTP request string
/// @return Encoded raw HTTP response string
string *basic_http_server(string *request);

#endif
