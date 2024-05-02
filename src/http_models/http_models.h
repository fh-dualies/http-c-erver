#ifndef HTTP_MODELS_H
#define HTTP_MODELS_H

#include "../../lib/string_lib/string_lib.h"

struct request_t {
  string *method;
  string *resource;
  string *version;
  string *host;
  string *user_agent;
  string *accept;
  string *connection;
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

/**
 * @brief Create a new request object
 * @warning This function will allocate memory for the request object and all its fields
 *
 * Returns NULL if memory allocation fails
 *
 * @return Created request object
 */
request_t *new_request();

/**
 * @brief Create a new response object
 * @warning This function will allocate memory for the response object and all its fields
 *
 * Returns NULL if memory allocation fails
 *
 * @return Created response object
 */
response_t *new_response();

/**
 * @brief Free the memory allocated for a request object
 * @warning This function will check if the given pointer is NULL
 *
 * Returns if the request object is NULL
 *
 * @param request Request object to be freed
 */
void free_request(request_t **request);

/**
 * @brief Free the memory allocated for a response object
 * @warning This function will check if the given pointer is NULL
 *
 * Returns if the response object is NULL
 *
 * @param response Response object to be freed
 */
void free_response(response_t **response);

/**
 * @brief Update the content length of a response object
 *
 * The length is calculated based on the current body length of the response.
 * Returns if the response object is NULL
 *
 * @param response Response object to be updated
 */
void update_response_content_length(response_t *response);

/**
 * @brief Add a header to a raw HTTP response string
 * @warning This function will not add the semicolon at between the header and value
 * @warning This function will add \r\n at the end of the header
 * @example Server: C-Webserver
 *
 * Returns if one of the raw strings is NULL
 *
 * @param raw_string Raw HTTP response string
 * @param header Header to be added (should be a string constant - null terminated)
 * @param value Value of the header
 */
void add_response_string_header(string *raw_string, const char *header, string *value);

/**
 * @brief Generate the status line of a response object
 * @warning This function will only fill the version, status code and status message fields to the
 * response object. Use generate_response_status_line to generate the full raw status line.
 *
 * Returns if the response object is NULL
 *
 * @param response Response object to be updated
 * @param status_code HTTP status code
 * @param content_type Content type of the response
 */
void generate_response_status(response_t *response, int status_code, string *content_type);

/**
 * @brief Generate the status line of a raw HTTP response string
 * @warning This function will add \r\n at the end of the status line
 * @example HTTP/1.1 200 OK
 *
 * Returns if the raw string is NULL
 *
 * @param raw_string Raw HTTP response string to be updated
 * @param version HTTP version
 * @param status_code HTTP status code
 * @param status_message HTTP status message
 */
void generate_response_status_line(string *raw_string, string *version, string *status_code,
                                   string *status_message);

#endif