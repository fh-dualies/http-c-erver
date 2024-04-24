#include "http_parser.h"
#include "../http_server/http_server.h"

// TODO: check for \0
int parse_request_line(string *raw_request, request_t *request) {
  enum { METHOD, RESOURCE, VERSION } segment = METHOD;
  size_t segment_start = 0;
  string *segments[] = {request->method, request->resource, request->version};

  for (size_t i = 0; i <= raw_request->len; i++) {
    unsigned char current = (i < raw_request->len) ? (char)(raw_request->str[i]) : '\n';

    if (segment > VERSION) {
      break;
    }

    if (current != ' ' && current != '\n' && current != '\r') {
      continue;
    }

    string *current_segment = segments[segment];

    if (current_segment == NULL) {
      free_request(&request);
      return EXIT_FAILURE;
    }

    str_cat(current_segment, raw_request->str + segment_start, i - segment_start);

    segment++;
    segment_start = i + 1;
  }

  return EXIT_SUCCESS;
}

void map_header(string *header_name, string *header_value, request_t *request) {
  if (header_name == NULL || header_value == NULL || request == NULL) {
    return;
  }

  if (str_cmp(header_name, REQUEST_HEADER_HOST) == 0) {
    str_set(request->host, header_value->str, header_value->len);
    return;
  }

  if (str_cmp(header_name, REQUEST_HEADER_USER_AGENT) == 0) {
    str_set(request->user_agent, header_value->str, header_value->len);
    return;
  }

  if (str_cmp(header_name, REQUEST_HEADER_ACCEPT) == 0) {
    str_set(request->accept, header_value->str, header_value->len);
    return;
  }

  if (str_cmp(header_name, REQUEST_HEADER_CONNECTION) == 0) {
    str_set(request->connection, header_value->str, header_value->len);
    return;
  }
}

// TODO: check for \0
int parse_request_headers(string *raw_request, request_t *request) {
  if (raw_request == NULL || request == NULL) {
    return EXIT_FAILURE;
  }

  const char *request_headers[REQUEST_HEADER_COUNT] = {
      REQUEST_HEADER_HOST, REQUEST_HEADER_USER_AGENT, REQUEST_HEADER_ACCEPT,
      REQUEST_HEADER_CONNECTION};

  string *header_name = _new_string();
  string *current_line = _new_string();
  size_t current_line_pos = 0;

  for (size_t i = 0; i < REQUEST_HEADER_COUNT; ++i) {
    /// @note strlen() is safe to use here - request_headers are constants defined in http_parser.h
    str_set(header_name, request_headers[i], strlen(request_headers[i]));
    str_to_lower(header_name);

    // TODO: Would it make sense to factor out the loop into a separate function? Just for
    // readability purposes
    for (size_t j = 0; j < raw_request->len; ++j) {
      char current = raw_request->str[j];

      if (current != '\r' && raw_request->str[j + 1] != '\n') {
        continue;
      }

      if (current_line_pos >= j) {
        break;
      }

      str_set(current_line, raw_request->str + current_line_pos, j - current_line_pos);
      str_to_lower(current_line);

      current_line_pos = j + 2;

      // TODO: Ensure that the header starts at the beginning of the line
      // TODO: remove strstr()
      // Think about the case where the header is not at the beginning of the line
      if (strstr(current_line->str, header_name->str) == NULL) {
        continue;
      }

      // TODO: remove strlen()
      // Think about checking for ": " instead of just moving 2 characters
      size_t header_name_end = strlen(header_name->str) + 2;
      string *header_value = _new_string();

      str_set(header_value, current_line->str + header_name_end,
              current_line->len - header_name_end);

      map_header(header_name, header_value, request);
      current_line_pos = 0;
      free_str(header_value);

      break;
    }
  }

  free_str(header_name);
  free_str(current_line);

  return EXIT_SUCCESS;
}

request_t *parse_request_string(string *raw_request) {
  if (raw_request == NULL) {
    return NULL;
  }

  request_t *request = new_request();

  if (request == NULL) {
    return NULL;
  }

  int result_line = parse_request_line(raw_request, request);

  if (result_line == EXIT_FAILURE) {
    free_request(&request);
    return NULL;
  }

  int result_header = parse_request_headers(raw_request, request);

  if (result_header == EXIT_FAILURE) {
    free_request(&request);
    return NULL;
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

  /// @node strlen() is safe to use here - HTTP_LINE_BREAK is a constant defined in http_parser.h
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

      // TODO: fix Clang-Tidy warning
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