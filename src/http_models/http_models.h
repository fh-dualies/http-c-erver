#ifndef HTTP_MODELS_H
#define HTTP_MODELS_H

#include "../../lib/http-lib/httplib.h"

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

/// @brief Create a new request object
/// @return Created request object
request_t *new_request();

/// @brief Create a new response object
/// @return Created response object
response_t *new_response();

/// @brief Free the memory allocated for a request object
/// @param request request object to be freed
void free_request(request_t *request);

/// @brief Free the memory allocated for a response object
/// @param response response object to be freed
void free_response(response_t *response);

/// @brief Update the content length of a response object
/// @param response Response object to be updated
void update_content_length(response_t *response);

/// @brief Build the status line of a response object
/// @param response Response object to be updated
/// @param status_code HTTP status code
/// @param content_type Content type of the response
void build_response_status(response_t *response, int status_code, const char *content_type);

/// @brief Add a header to a raw HTTP response string
/// @param raw_string Raw HTTP response string
/// @param header Header to be added
/// @param value Value of the header
void add_string_header(string *raw_string, const char *header, string *value);

/// @brief Build the status line of a raw HTTP string
/// @param raw_string Raw HTTP string to be updated
/// @param version HTTP version
/// @param status_code HTTP status code
/// @param status_message HTTP status message
void build_response_status_line(string *raw_string, string *version, string *status_code,
                                string *status_message);

#endif