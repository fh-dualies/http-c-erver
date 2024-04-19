#include "http_parser.h"
#include "../http_server/http_server.h"

request_t *parse_request_string(string *raw_request) {
  if (raw_request == NULL) {
    return NULL;
  }

  request_t *request = new_request();

  if (request == NULL) {
    return NULL;
  }

  // parse request line
  int segment = 0;                // 0: method, 1: resource, 2: version
  size_t segment_start = 0;       // start index of current segment
  string *current_segment = NULL; // current segment string

  for (size_t i = 0; i < raw_request->len; i++) {
    char current = raw_request->str[i];

    // check if we have reached the end of the request line
    if (segment > 2) {
      break;
    }

    // skip in-segment characters
    if (current != ' ' && current != '\n' && current != '\r') {
      continue;
    }

    // determine current segment pointer
    switch (segment) {
    case 0:
      current_segment = request->method;
      break;
    case 1:
      current_segment = request->resource;
      break;
    case 2:
      current_segment = request->version;
      break;
    default:
      break;
    }

    if (current_segment == NULL) {
      free_request(&request);
      return NULL;
    }

    // copy segment to request object
    str_cat(current_segment, raw_request->str + segment_start, i - segment_start);

    // move to next segment
    segment++;
    segment_start = i + 1;
    current_segment = NULL;
  }

  return request;
}

string *serialize_response(response_t *response) {
  if (response == NULL) {
    return NULL;
  }

  string *encoded_response = _new_string();

  if (encoded_response == NULL) {
    return NULL;
  }

  // status line (version, status code, status message)
  generate_response_status_line(encoded_response, response->version, response->status_code,
                                response->status_message);

  // headers
  add_response_string_header(encoded_response, CONTENT_TYPE_HEADER, response->content_type);
  add_response_string_header(encoded_response, CONTENT_LENGTH_HEADER, response->content_length);
  add_response_string_header(encoded_response, SERVER_HEADER, response->server);

  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  // body
  str_cat(encoded_response, response->body->str, response->body->len);

  return encoded_response;
}

string *decode_url(string *str) {
  if (str == NULL) {
    return NULL;
  }

  string *decoded = _new_string();

  if (decoded == NULL) {
    return NULL;
  }

  for (int i = 0; i < str->len; i++) {
    char current = str->str[i];

    // plus signs should be decoded as spaces
    if (current == '+') {
      str_cat(decoded, " ", 1);
      continue;
    }

    if (current == '%' && i + 1 < str->len) {
      char new_char = '\0';
      char first = str->str[i + 1];
      char second = str->str[i + 2];

      if (first >= '0' && first <= '9') {
        new_char = (char)(first - '0');
      } else if (first >= 'a' && first <= 'f') {
        new_char = (char)(first - 'a' + 10);
      } else if (first >= 'A' && first <= 'F') {
        new_char = (char)(first - 'A' + 10);
      }

      new_char <<= 4;

      if (second >= '0' && second <= '9') {
        new_char |= (char)(second - '0');
      } else if (second >= 'a' && second <= 'f') {
        new_char |= (char)(second - 'a' + 10);
      } else if (second >= 'A' && second <= 'F') {
        new_char |= (char)(second - 'A' + 10);
      }

      str_cat(decoded, &new_char, 1);
      i += 2;

      continue;
    }

    // other characters should be copied
    str_cat(decoded, str->str + i, 1);
  }

  return decoded;
}

string *encode_url(string *str) {
  if (str == NULL) {
    return NULL;
  }

  string *encoded = _new_string();

  if (encoded == NULL) {
    return NULL;
  }

  const char *hex = HEX_CHARSET;

  for (int i = 0; i < str->len; i++) {
    char current = str->str[i];

    // normal characters should be copied
    if (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z' ||
        current >= '0' && current <= '9') {
      str_cat(encoded, str->str + i, 1);
      continue;
    }

    // spaces should be encoded as '+'
    if (current == ' ') {
      str_cat(encoded, "+", 1);
      continue;
    }

    // other characters should be encoded as '%XX'
    str_cat(encoded, "%", 1);
    str_cat(encoded, hex + (current >> 4), 1);
    str_cat(encoded, hex + (current & 0x0f), 1);
  }

  return encoded;
}