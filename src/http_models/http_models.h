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

/**
 * @brief Create a new request object
 *
 * Returns NULL if memory allocation fails
 *
 * @return Created request object
 */
request_t *new_request();

/**
 * @brief Create a new response object
 *
 * Returns NULL if memory allocation fails
 *
 * @return Created response object
 */
response_t *new_response();

/**
 * @brief Free the memory allocated for a request object
 * @warning This function does not check if the given pointer is NULL
 *
 * Returns if the request object is NULL
 *
 * @param request Request object to be freed
 */
void free_request(request_t **request);

/**
 * @brief Free the memory allocated for a response object
 * @warning This function does not check if the given pointer is NULL
 *
 * Returns if the response object is NULL
 *
 * @param response Response object to be freed
 */
void free_response(response_t **response);

/**
 * @brief Update the content length of a response object
 *
 * Returns if the response object is NULL
 *
 * @param response Response object to be updated
 */
void update_response_content_length(response_t *response);

/**
 * @brief Add a header to a raw HTTP response string
 *
 * Returns if the raw string is NULL
 *
 * @param raw_string Raw HTTP response string
 * @param header Header to be added
 * @param value Value of the header
 */
void add_response_string_header(string *raw_string, const char *header, string *value);

/**
 * @brief Generate the status line of a response object
 *
 * Returns if the response object is NULL
 *
 * @param response Response object to be updated
 * @param status_code HTTP status code
 * @param content_type Content type of the response
 */
void generate_response_status(response_t *response, int status_code, const char *content_type);

/**
 * @brief Generate the status line of a raw HTTP response string
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