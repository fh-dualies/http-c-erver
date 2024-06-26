#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "../http_models/http_models.h"

#define HEX_CHARSET "0123456789ABCDEF"

/// @warning has to be in lowercase!
#define REQUEST_HEADER_COUNT 4
#define REQUEST_HEADER_HOST "host:"
#define REQUEST_HEADER_USER_AGENT "user-agent:"
#define REQUEST_HEADER_ACCEPT "accept:"
#define REQUEST_HEADER_CONNECTION "connection:"

/**
 * @brief Parse the request line of a raw HTTP request string
 *
 * Returns EXIT_FAILURE if the raw_request or request is NULL, if memory allocation fails or if the
 * request line is invalid. The raw_request is seen as invalid if it does not follow the HTTP/1.1
 * (or 1.0) request format: `METHOD RESOURCE VERSION\r\n`.
 *
 * @param raw_request Raw HTTP request string
 * @param request Request object to store the parsed data
 * @return int EXIT_SUCCESS if the request line was parsed successfully, EXIT_FAILURE otherwise
 */
int parse_request_line(string *raw_request, request_t *request);

/**
 * @brief Map a header name and value to the corresponding field in a request object
 *
 * If the header_name is NULL, the header_value is NULL or the request is NULL, the function will
 * return without doing anything.
 *
 * Implemented headers:
 * - Host: request->host
 * - User-Agent: request->user_agent
 * - Accept: request->accept
 * - Connection: request->connection
 *
 * @param header_name Header name
 * @param header_value Header value
 * @param request Request object to store the mapped data
 */
void map_header(string *header_name, string *header_value, request_t *request);

/**
 * @brief Split the raw request string into the head and the body
 * Split the raw request at \r\n\r\n and return the head.
 *
 * Returns NULL if the raw_request is NULL.
 *
 * @param raw_request Raw HTTP request string
 * @return New string with the head
 */
string *get_request_head(string *raw_request);

/**
 * @brief Parse the request line of a raw HTTP request string
 *
 * Returns EXIT_FAILURE if the raw_request or request is NULL, if memory allocation fails or if the
 * request line is invalid.
 *
 * @param raw_request Raw HTTP request string
 * @param request Request object to store the parsed data
 * @return int EXIT_SUCCESS if the request line was parsed successfully, EXIT_FAILURE otherwise
 */
int parse_request_headers(string *raw_request, request_t *request);

/**
 * @brief Parse a raw HTTP request string into a request object
 * @warning The request object must be freed with free_request() after use
 *
 * Returns NULL if the raw_request is NULL, if memory allocation fails or if the request line is
 * invalid. The raw_request is seen as invalid if it does not follow the HTTP/1.1 (or 1.0) request
 * format: `METHOD RESOURCE VERSION\r\n`. Additionally, it will fail if the headers are not in the
 * format `HEADER: VALUE\r\n`.
 *
 * @param raw_request Raw HTTP request string
 * @return request_t* Decoded request object
 */
request_t *parse_request_string(string *raw_request);

/**
 * @brief Encode a response object into a raw HTTP response string
 * @warning The returned string must be freed with free_string() after use
 *
 * Returns NULL if the response is NULL or if memory allocation fails.
 * The response is encoded in the following format: `VERSION STATUS_CODE STATUS_MESSAGE\r\nHEADER1:
 * VALUE1\r\nHEADER2: VALUE2\r\n...\r\n\r\nBODY`.
 *
 * @param response Response object to be encoded
 * @return string* Encoded raw HTTP response string
 */
string *serialize_response(response_t *response);

/**
 * @brief URL decode a string
 *
 * Returns NULL if the str is NULL or if memory allocation fails.
 *
 * @param str The string to decode
 * @return string* The decoded string
 */
string *decode_url(string *str);

/**
 * @brief URL encode a string
 *
 * Returns NULL if the str is NULL or if memory allocation fails.
 *
 * @param str The string to encode
 * @return string* The encoded string
 */
string *encode_url(string *str);

#endif