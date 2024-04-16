#include "http_server.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

/// @brief Create a new request object
/// @return Created request object
request_t *new_request() {
  request_t *request = malloc(sizeof(struct request_t));

  if (request == NULL) {
    return NULL;
  }

  request->method = _new_string();
  request->resource = _new_string();
  request->version = _new_string();

  if (request->method == NULL || request->resource == NULL || request->version == NULL) {
    free(request);
    return NULL;
  }

  return request;
}

/// @brief Create a new response object
/// @return Created response object
response_t *new_response() {
  response_t *response = malloc(sizeof(struct response_t));

  if (response == NULL) {
    return NULL;
  }

  response->version = _new_string();
  response->status_code = _new_string();
  response->status_message = _new_string();
  response->content_type = _new_string();
  response->content_length = _new_string();
  response->server = _new_string();
  response->body = _new_string();

  if (response->version == NULL || response->status_code == NULL ||
      response->content_type == NULL || response->content_length == NULL ||
      response->body == NULL || response->server == NULL) {
    free(response);
    return NULL;
  }

  return response;
}

/// @brief Free the memory allocated for a request object
/// @param request request object to be freed
void free_request(request_t *request) {
  if (request == NULL) {
    return;
  }

  free_str(request->resource);
  free_str(request->version);
  free_str(request->method);
  free(request);
}

/// @brief Free the memory allocated for a response object
/// @param response response object to be freed
void free_response(response_t *response) {
  if (response == NULL) {
    return;
  }

  free_str(response->version);
  free_str(response->status_code);
  free_str(response->status_message);
  free_str(response->content_type);
  free_str(response->content_length);
  free_str(response->server);
  free_str(response->body);
  free(response);
}

/// @brief Clean up memory allocated to exit the http server
/// @param request request object to be freed
/// @param response response object to be freed
void cleanup(request_t *request, response_t *response) {
  free_request(request);
  free_response(response);
}

/// @brief Decode a raw HTTP request string into a request object
/// @param raw_request Raw HTTP request string
/// @return Decoded request object
request_t *decode_request_string(string *raw_request) {
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
      free_request(request);
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

/// @brief Encode a response object into a raw HTTP response string
/// @param response response object to be encoded
/// @param error Flag indicating if the response is an error response
/// @return Encoded raw HTTP response string
string *encode_response(response_t *response) {
  if (response == NULL) {
    return NULL;
  }

  string *encoded_response = _new_string();

  if (encoded_response == NULL) {
    return NULL;
  }

  // status line (version, status code, status message)
  str_cat(encoded_response, response->version->str, response->version->len);
  str_cat(encoded_response, " ", 1);
  str_cat(encoded_response, response->status_code->str, response->status_code->len);

  str_cat(encoded_response, " ", 1);
  str_cat(encoded_response, response->status_message->str, response->status_message->len);

  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  // headers
  str_cat(encoded_response, CONTENT_TYPE_HEADER, strlen(CONTENT_TYPE_HEADER));
  str_cat(encoded_response, response->content_type->str, response->content_type->len);
  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  str_cat(encoded_response, CONTENT_LENGTH_HEADER, strlen(CONTENT_LENGTH_HEADER));
  str_cat(encoded_response, response->content_length->str, response->content_length->len);
  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  str_cat(encoded_response, SERVER_HEADER, strlen(SERVER_HEADER));
  str_cat(encoded_response, response->server->str, response->server->len);
  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  str_cat(encoded_response, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));

  // body
  str_cat(encoded_response, response->body->str, response->body->len);

  return encoded_response;
}

/// @brief Check if a given path is valid
/// @param path Path to be checked
/// @return True if the path is valid, false otherwise
bool verify_path(char *path) {
  if (path == NULL) {
    return false;
  }

  if (strlen(path) == 0) {
    return false;
  }

  if (path[0] != '/') {
    return false;
  }

  // should contain DOCUMENT_ROOT (mainly prevents access to other directories)
  if (strstr(path, DOCUMENT_ROOT) == NULL) {
    return false;
  }

  // check if path contains ".." (just to be sure)
  if (strstr(path, "..") != NULL) {
    return false;
  }

  return true;
}

/// @brief Verify if a given request is valid
/// @param request Request to be verified
/// @return True if the request is valid, false otherwise
bool verify_decoded_request(request_t *request) {
  if (request->method == NULL || request->resource == NULL || request->version == NULL) {
    return false;
  }

  if (request->method->len == 0 || request->resource->len == 0 || request->version->len == 0) {
    return false;
  }

  // check if http version is 1.0 or 1.1
  if (strcmp(request->version->str, HTTP_VERSION_1_0) != 0 &&
      strcmp(request->version->str, HTTP_VERSION_1_1) != 0) {
    return false;
  }

  return true;
}

/// @brief Gets the HTTP status message for a given status code
/// @param status_code HTTP status code
/// @return HTTP status message
const char *get_http_status_message(int status_code) {
  // List of HTTP status codes:
  // https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  switch (status_code) {
  case HTTP_OK:
    return STATUS_MESSAGE_OK;
  case HTTP_BAD_REQUEST:
    return STATUS_MESSAGE_BAD_REQUEST;
  case HTTP_FORBIDDEN:
    return STATUS_MESSAGE_FORBIDDEN;
  case HTTP_NOT_FOUND:
    return STATUS_MESSAGE_NOT_FOUND;
  case HTTP_INTERNAL_SERVER_ERROR:
    return STATUS_MESSAGE_INTERNAL_SERVER_ERROR;
  case HTTP_NOT_IMPLEMENTED:
    return STATUS_MESSAGE_NOT_IMPLEMENTED;
  default:
    return STATUS_MESSAGE_UNKNOWN;
  }
}

string *debug_response(request_t *request) {
  response_t *response = new_response();

  if (response == NULL) {
    return NULL;
  }

  response->version = str_set(response->version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));

  string *status_code = int_to_string(HTTP_OK);
  response->status_code = str_set(response->status_code, status_code->str, status_code->len);
  free_str(status_code);

  response->status_message = str_set(response->status_message, get_http_status_message(HTTP_OK),
                                     strlen(get_http_status_message(HTTP_OK)));
  response->content_type =
      str_set(response->content_type, CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
  response->server = str_set(response->server, SERVER_SIGNATURE, strlen(SERVER_SIGNATURE));

  // HTML body
  response->body = str_set(response->body, "<html><head><title>Debug</title></head><body>", 45);
  response->body = str_cat(response->body, "<p>HTTP-Methode: ", 17);
  response->body = str_cat(response->body, request->method->str, request->method->len);

  response->body = str_cat(response->body, "<br>Ressource: ", 15);
  response->body = str_cat(response->body, request->resource->str, request->resource->len);

  response->body = str_cat(response->body, "<br>HTTP-Version: ", 18);
  response->body = str_cat(response->body, request->version->str, request->version->len);
  response->body = str_cat(response->body, "</p></body></html>", 18);

  // Content length
  string *content_length = size_t_to_string(response->body->len);
  response->content_length =
      str_set(response->content_length, content_length->str, content_length->len);
  free_str(content_length);

  // Encode response
  string *encoded_response = encode_response(response);

  free_response(response);
  free_request(request);

  return encoded_response;
}

/// @brief Create error response for a given status code
/// @param status_code HTTP status code
/// @return Encoded raw HTTP response string
string *error_response(int status_code) {
  response_t *response = new_response();

  if (response == NULL) {
    return NULL;
  }

  const char *status_message = get_http_status_message(status_code);

  response->version = str_set(response->version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));
  // is getting freed later because it is used again later
  string *status_code_str = int_to_string(status_code);

  response->status_code =
      str_set(response->status_code, status_code_str->str, status_code_str->len);
  response->status_message =
      str_set(response->status_message, status_message, strlen(status_message));
  response->server = str_set(response->server, SERVER_SIGNATURE, strlen(SERVER_SIGNATURE));
  response->content_type =
      str_set(response->content_type, CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));

  response->body = str_set(response->body, "<html><head><title>Error</title></head><body><h1>", 49);
  str_cat(response->body, status_code_str->str, status_code_str->len);
  free_str(status_code_str);

  str_cat(response->body, "</h1><p>", 8);
  str_cat(response->body, status_message, strlen(status_message));
  str_cat(response->body, "</p></body></html>", 18);

  string *content_length = size_t_to_string(response->body->len);
  response->content_length =
      str_set(response->content_length, content_length->str, content_length->len);
  free_str(content_length);

  string *encoded_response = encode_response(response);

  free_response(response);

  return encoded_response;
}

string *http_server(string *raw_request) {
  request_t *decoded_request = decode_request_string(raw_request);

  if (decoded_request == NULL) {
    return error_response(HTTP_BAD_REQUEST);
  }

  // validate request content
  if (!verify_decoded_request(decoded_request)) {
    free_request(decoded_request);
    return error_response(HTTP_BAD_REQUEST);
  }

  // check if method is implemented
  if (strcmp(decoded_request->method->str, HTTP_METHOD_GET) != 0) {
    cleanup(decoded_request, NULL);
    return error_response(HTTP_NOT_IMPLEMENTED);
  }

  // return debug response if requested
  if (strcmp(decoded_request->resource->str, "/debug") == 0) {
    // no cleanup needed, debug_response() will free the request
    return debug_response(decoded_request);
  }

  response_t *response = new_response();

  if (response == NULL) {
    free_request(decoded_request);
    return error_response(HTTP_INTERNAL_SERVER_ERROR);
  }

  // create path to resource
  char *absolute_path = get_absolute_path(decoded_request->resource);

  // check if resource exists and get absolute path
  if (absolute_path == NULL) {
    cleanup(decoded_request, response);
    return error_response(HTTP_NOT_FOUND);
  }

  printf("path: %s\n", absolute_path);

  // check if path is valid
  if (!verify_path(absolute_path)) {
    cleanup(decoded_request, response);
    return error_response(HTTP_FORBIDDEN);
  }

  string *file_content = read_file(absolute_path);

  if (file_content == NULL) {
    int error = HTTP_NOT_FOUND;

    // check if file access was denied
    if (errno == EACCES) {
      error = HTTP_FORBIDDEN;
    }

    cleanup(decoded_request, response);
    return error_response(error);
  }

  // fill response object
  response->version = str_set(response->version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));

  string *status_code = int_to_string(HTTP_OK);
  response->status_code = str_set(response->status_code, status_code->str, status_code->len);
  free_str(status_code);

  response->status_message = str_set(response->status_message, get_http_status_message(HTTP_OK),
                                     strlen(get_http_status_message(HTTP_OK)));
  response->content_type =
      str_set(response->content_type, CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
  response->server = str_set(response->server, SERVER_SIGNATURE, strlen(SERVER_SIGNATURE));

  string *content_length = size_t_to_string(file_content->len);
  response->content_length =
      str_set(response->content_length, content_length->str, content_length->len);
  free_str(content_length);

  // needs to br freed so we can update reference
  free_str(response->body);
  response->body = file_content;

  string *encoded_response = encode_response(response);

  cleanup(decoded_request, response);
  free(content_length);

  return encoded_response;
}