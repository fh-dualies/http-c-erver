#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../lib/http-lib/httplib.h"

// Server Constants
#define SERVER_SIGNATURE "LLDM/0.1 Basic HTTP Server"
#define DOCUMENT_ROOT "src/root" // TODO: make this cleaner? Issue:
// https://git.fh-muenster.de/pse2024/PG5_1/pse-2024/-/issues/4
// Set to `../src/root/` when running the program with jetbrains (clion)
// Set to `src/root/` when running the program manually

// HTTP Constants
#define HTTP_VERSION_1_0 "HTTP/1.0"
#define HTTP_VERSION_1_1 "HTTP/1.1"
#define CONTENT_TYPE_HTML "text/html"
#define HTTP_LINE_BREAK "\r\n"
#define HTTP_METHOD_GET "GET"

// HTTP Status Codes
#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501

// HTTP Status Messages
#define STATUS_MESSAGE_OK "OK"
#define STATUS_MESSAGE_BAD_REQUEST "Bad Request"
#define STATUS_MESSAGE_FORBIDDEN "Forbidden"
#define STATUS_MESSAGE_NOT_FOUND "Not Found"
#define STATUS_MESSAGE_INTERNAL_SERVER_ERROR "Internal Server Error"
#define STATUS_MESSAGE_NOT_IMPLEMENTED "Not Implemented"
#define STATUS_MESSAGE_UNKNOWN "Unknown"

// HTTP Headers
#define CONTENT_LENGTH_HEADER "Content-Length: "
#define CONTENT_TYPE_HEADER "Content-Type: "
#define SERVER_HEADER "Server: "

struct request_t {
  string *method;
  string *resource;
  string *version;
} typedef request_t;

struct response_t {
  string *version;
  string *status_code;
  string *status_message;
  string *content_type;
  string *content_length;
  string *server;
  string *body;
} typedef response_t;

/// @breif Starts a basic HTTP server
/// @param request Raw HTTP request string
/// @return Encoded raw HTTP response string
string *http_server(string *request);

#endif
