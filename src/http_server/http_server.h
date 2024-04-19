#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../lib/http-lib/httplib.h"

// Server Constants
#define SERVER_SIGNATURE "LLDM/0.1 Basic HTTP Server"
#define DOCUMENT_ROOT "src/htdocs" // TODO: make this cleaner?

// Server Routes
#define ROUTE_DEBUG "/debug"

// HTTP Constants
#define HTTP_VERSION_1_0 "HTTP/1.0"
#define HTTP_VERSION_1_1 "HTTP/1.1"
#define HTTP_LINE_BREAK "\r\n"
#define HTTP_METHOD_GET "GET"

// HTTP Content Types
#define CONTENT_TYPE_HTML "text/html"
#define CONTENT_TYPE_CSS "text/css"
#define CONTENT_TYPE_JS "application/javascript"
#define CONTENT_TYPE_JPEG "image/jpeg"
#define CONTENT_TYPE_PNG "image/png"
#define CONTENT_TYPE_ICO "image/x-icon"
#define CONTENT_TYPE_TEXT "text/plain"

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

// File extensions
#define EXTENSION_HTML ".html"
#define EXTENSION_CSS ".css"
#define EXTENSION_JS ".js"
#define EXTENSION_JPG ".jpg"
#define EXTENSION_JPEG ".jpeg"
#define EXTENSION_PNG ".png"
#define EXTENSION_ICO ".ico"

/// @brief Gets the HTTP status message for a given status code
/// @param status_code HTTP status code
/// @return HTTP status message
const char *get_http_status_message(int status_code);

/// @breif Starts a basic HTTP server
/// @param request Raw HTTP request string
/// @return Encoded raw HTTP response string
string *http_server(string *request);

#endif
