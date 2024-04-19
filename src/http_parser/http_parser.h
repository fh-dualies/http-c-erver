#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "../http_models/http_models.h"

#define HEX_CHARSET "0123456789ABCDEF"

/**
 * @brief Parse a raw HTTP request string into a request object
 * @warning The request object must be freed with free_request() after use
 *
 * Returns NULL if the raw_request is NULL, if memory allocation fails or if the request line is
 * invalid. The raw_request is seen as invalid if it does not follow the HTTP/1.1 (or 1.0) request
 * format: `METHOD RESOURCE VERSION\r\n`.
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