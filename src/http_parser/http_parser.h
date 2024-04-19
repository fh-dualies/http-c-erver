#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "../http_models/http_models.h"

#define HEX_CHARSET "0123456789ABCDEF"

/// @brief Decode a raw HTTP request string into a request object
/// @param raw_request Raw HTTP request string
/// @return Decoded request object
request_t *parse_request_string(string *raw_request);

/// @brief Encode a response object into a raw HTTP response string
/// @param response response object to be encoded
/// @param error Flag indicating if the response is an error response
/// @return Encoded raw HTTP response string
string *serialize_response(response_t *response);

/// @brief URL decode a string
/// @param str The string to decode
/// @return The decoded string
string *url_decode(string *str);

/// @brief URL encode a string
/// @param str The string to encode
/// @return The encoded string
string *url_encode(string *str);

#endif