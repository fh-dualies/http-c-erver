#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../lib/http-lib/httplib.h"
#include "../http_parser/http_parser.h"
#include <stdbool.h>

// Server Constants
#define SERVER_SIGNATURE "LLDM/0.1 HTTP Server"

// HTTP Constants
/// @warning This path is relative to the project root - depending on the build
/// system, the path may need to be adjusted. For example, running the server.out in
/// the cli requires no change, but running the server via jetbrains IDE's
/// requires the path to be set to "../src/htdocs"
#define DOCUMENT_ROOT "src/htdocs"
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
#define HTTP_VERSION_NOT_SUPPORTED 505

// HTTP Status Messages
#define STATUS_MESSAGE_OK "OK"
#define STATUS_MESSAGE_BAD_REQUEST "Bad Request"
#define STATUS_MESSAGE_FORBIDDEN "Forbidden"
#define STATUS_MESSAGE_NOT_FOUND "Not Found"
#define STATUS_MESSAGE_INTERNAL_SERVER_ERROR "Internal Server Error"
#define STATUS_MESSAGE_NOT_IMPLEMENTED "Not Implemented"
#define STATUS_MESSAGE_UNKNOWN "Unknown"
#define STATUS_MESSAGE_VERSION_NOT_SUPPORTED "HTTP Version Not Supported"

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

/**
 * @brief Clean up memory allocated to exit the http server
 * @warning This function does not check if the given pointers are NULL
 *
 * @param request request object to be freed
 * @param response response object to be freed
 * @param path path to be freed
 */
void cleanup(request_t **request, response_t **response, char *path);

/**
 * @brief Get the mime type of a file
 * @warning path should not be a struct string
 *
 * The mime type is determined by the file extension.
 * If the file extension is not known, the default mime type is text/plain.
 *
 * @param path Path to the file
 * @return Mime type of the file
 */
const char *get_mime_type(const char *path);

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
