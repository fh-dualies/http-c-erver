#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "../http_models/http_models.h"

#define HEX_CHARSET "0123456789ABCDEF"

/**
 * @brief Parse a raw HTTP request string into a request object
 *
 * Returns NULL if the raw_request is NULL, if memory allocation fails or if the request line is
 * invalid.
 *
 * @param raw_request Raw HTTP request string
 * @return request_t* Decoded request object
 */
request_t *parse_request_string(string *raw_request);

/**
 * @brief Encode a response object into a raw HTTP response string
 *
 * Returns NULL if the response is NULL or if memory allocation fails.
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