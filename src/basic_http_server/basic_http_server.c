#include "basic_http_server.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

basic_request *new_request() {
  basic_request *request = malloc(sizeof(basic_request));

  if (request == NULL) {
    return NULL;
  }

  request->method = _new_string();
  request->resource = _new_string();
  request->version = _new_string();

  return request;
}

basic_response *new_response() {
  basic_response *response = malloc(sizeof(basic_response));

  if (response == NULL) {
    return NULL;
  }

  response->version = _new_string();
  response->status_code = _new_string();
  response->status_message = _new_string();
  response->content_type = _new_string();
  response->content_length = _new_string();
  response->body = _new_string();

  return response;
}

void free_request(basic_request *request) {
  if (request == NULL) {
    return;
  }

  free_str(request->resource);
  free_str(request->version);
  free_str(request->method);
  free(request);
}

void free_response(basic_response *response) {
  if (response == NULL) {
    return;
  }

  free_str(response->version);
  free_str(response->status_code);
  free_str(response->status_message);
  free_str(response->content_type);
  free_str(response->content_length);
  free_str(response->body);
  free(response);
}

basic_request *decode_request_string(string *raw_request) {
  if (raw_request == NULL) {
    return NULL; // TODO: Error - Raw request is NULL (400)
  }

  basic_request *request = new_request();

  if (request == NULL) {
    return NULL; // TODO: Error - Unable to create request (500)
  }

  int segment = 0;
  size_t segment_start = 0;
  string *current_segment = NULL;

  for (size_t i = 0; i < raw_request->len; i++) {
    char current = raw_request->str[i];

    if (segment > 2) {
      break;
    }

    if (current != ' ' && current != '\n' && current != '\r') {
      continue;
    }

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
      return NULL; // TODO: Error - Destination is NULL (500)
    }

    str_cat(current_segment, get_char_str(raw_request) + segment_start,
            i - segment_start);

    segment++;
    segment_start = i + 1;
    current_segment = NULL;
  }

  return request;
}

string *encode_response(basic_response *response) {
  string *encoded_response =
      cpy_str(get_char_str(response->version), get_length(response->version));

  // status line
  str_cat(encoded_response, " ", 1);
  str_cat(encoded_response, get_char_str(response->status_code),
          get_length(response->status_code));

  str_cat(encoded_response, " ", 1);
  str_cat(encoded_response, get_char_str(response->status_message),
          get_length(response->status_message));

  str_cat(encoded_response, "\n", 1);

  // headers
  str_cat(encoded_response, "Content-Type: ", 14);
  str_cat(encoded_response, get_char_str(response->content_type),
          get_length(response->content_type));
  str_cat(encoded_response, "\n", 1);

  str_cat(encoded_response, "Content-Length: ", 16);
  str_cat(encoded_response, get_char_str(response->content_length),
          get_length(response->content_length));
  str_cat(encoded_response, "\n", 1);

  str_cat(encoded_response, "\n", 1);

  // body
  str_cat(encoded_response, get_char_str(response->body),
          get_length(response->body));

  return encoded_response;
}

string *basic_http_server(string *request) {
  basic_request *decoded_request = decode_request_string(request);

  if (decoded_request == NULL) {
    return NULL; // TODO: Error - Unable to decode request (400)
  }

  basic_response *response = new_response();

  if (response == NULL) {
    return NULL; // TODO: Error - Unable to create response (500)
  }

  char relative_path[strlen(DOCUMENT_ROOT) + 12];
  char absolute_path[PATH_MAX];

  strcpy(relative_path, DOCUMENT_ROOT);
  strcat(relative_path, "index.html"); // TODO: read from request

  if (realpath(relative_path, absolute_path) == NULL) {
    return NULL; // TODO: Error - Unable to resolve path (404)
  }

  string *file_content = read_file(absolute_path);

  if (file_content == NULL) {
    return NULL; // TODO: Error - Unable to read file (404)
  }

  response->version = cpy_str(HTTP_VERSION, strlen(HTTP_VERSION));
  response->status_code = cpy_str("200", 3);
  response->status_message = cpy_str("OK", 2);
  response->content_type = cpy_str("text/html", 9);

  char content_length[file_content->len];
  sprintf(content_length, "%zu", file_content->len);

  response->content_length = cpy_str(content_length, strlen(content_length));
  response->body = file_content;

  string *encoded_response = encode_response(response);

  free_request(decoded_request);
  free_response(response);

  return encoded_response;
}