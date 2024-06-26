#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../lib/string_lib/string_lib.h"
#include "../http_parser/http_parser.h"
#include <stdbool.h>

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
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_VERSION_NOT_SUPPORTED 505

// HTTP Status Messages
#define STATUS_MESSAGE_OK "OK"
#define STATUS_MESSAGE_BAD_REQUEST "Bad Request"
#define STATUS_MESSAGE_UNAUTHORIZED "Authentication required"
#define STATUS_MESSAGE_FORBIDDEN "Forbidden"
#define STATUS_MESSAGE_NOT_FOUND "Not Found"
#define STATUS_MESSAGE_INTERNAL_SERVER_ERROR "Internal Server Error"
#define STATUS_MESSAGE_NOT_IMPLEMENTED "Not Implemented"
#define STATUS_MESSAGE_VERSION_NOT_SUPPORTED "HTTP Version Not Supported"
#define STATUS_MESSAGE_UNKNOWN "Unknown"

// HTTP Headers
#define CONTENT_LENGTH_HEADER "Content-Length: "
#define CONTENT_TYPE_HEADER "Content-Type: "
#define WWW_AUTHENTICATE_HEADER "WWW-Authenticate: "
#define SERVER_HEADER "Server: "

#define WWW_AUTHENTICATE_REALM "Basic realm=\"Secure Area\""

// File extensions
#define EXTENSION_HTML ".html"
#define EXTENSION_CSS ".css"
#define EXTENSION_JS ".js"
#define EXTENSION_JPG ".jpg"
#define EXTENSION_JPEG ".jpeg"
#define EXTENSION_PNG ".png"
#define EXTENSION_ICO ".ico"

/**
 * @brief Get the mime type of a file
 * @warning path should not be a struct string
 *
 * The mime type is determined by the file extension.
 * If the file extension is not known, the default mime type is text/plain.
 *
 * @param path Path to the file (constant string - null terminated)
 * @return Mime type of the file
 */
string *get_mime_type(const char *path);

/**
 * @brief Create an error response for a given status code
 *
 * The error response is created by generating a response object with the given status code.
 * The body of the response contains a simple HTML error message.
 *
 * @param status_code HTTP status code
 * @return Encoded raw HTTP response string
 */
string *error_response(int status_code);

/**
 * @brief Create a debug_route response for a given request
 * @warning This function will free the given request object
 *
 * The debug_route response is created by generating a response object with the status code 200.
 * The body of the response contains the HTTP method, resource and version of the request.
 *
 * @param request Request object to be debugged
 * @return Encoded raw HTTP response string
 */
string *debug_response(request_t *request);

/**
 * @brief Get HTTP status message for a given status code
 *
 * Implemented status codes: 200, 400, 403, 404, 500, 501
 * If the given status code is not implemented, the function will return "Unknown".
 *
 *
 * @param status_code HTTP status code
 * @return HTTP status message
 */
const char *get_http_status_message(int status_code);

/**
 * @brief Start a basic HTTP server
 *
 * The function processes a raw HTTP request and returns a raw HTTP response.
 * The function will return an error response if the request is invalid or the method is not
 * implemented. If the requested resource is "/debug_route", the function will return a debug_route
 * response. If the requested resource is not found, the function will return a 404 response. If the
 * requested resource is forbidden, the function will return a 403 response. If the requested
 * resource cannot be accessed, the function will return a 500 response.
 *
 * @param raw_request Raw HTTP request string
 * @return Encoded raw HTTP response string
 */
string *http_server(string *request);

#endif
