#include "http_server.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

void cleanup(request_t **request, response_t **response, char *path) {
  free_request(request);
  free_response(response);
  free(path);
}

bool is_valid_path(char *path) {
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

bool is_valid_request(request_t *request) {
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

const char *get_mime_type(char *path) {
  if (path == NULL) {
    return NULL;
  }

  const char *extension = strrchr(path, '.');

  if (extension == NULL) {
    return CONTENT_TYPE_TEXT;
  }

  if (strcmp(extension, EXTENSION_HTML) == 0) {
    return CONTENT_TYPE_HTML;
  }

  if (strcmp(extension, EXTENSION_CSS) == 0) {
    return CONTENT_TYPE_CSS;
  }

  if (strcmp(extension, EXTENSION_JS) == 0) {
    return CONTENT_TYPE_JS;
  }

  if (strcmp(extension, EXTENSION_JPG) == 0 || strcmp(extension, EXTENSION_JPEG) == 0) {
    return CONTENT_TYPE_JPEG;
  }

  if (strcmp(extension, EXTENSION_PNG) == 0) {
    return CONTENT_TYPE_PNG;
  }

  if (strcmp(extension, EXTENSION_ICO) == 0) {
    return CONTENT_TYPE_ICO;
  }

  return CONTENT_TYPE_TEXT;
}

char *convert_to_absolute_path(string *resource) {
  if (resource == NULL) {
    return NULL;
  }

  string *relative_path = _new_string();
  char *absolute_path = calloc(PATH_MAX, 1);

  if (absolute_path == NULL) {
    free_str(relative_path);
    return NULL;
  }

  // add document root to relative path
  str_cat(relative_path, DOCUMENT_ROOT, strlen(DOCUMENT_ROOT));
  // add resource to relative path
  str_cat(relative_path, resource->str, resource->len);

  // get real path of the resource
  char *real_path = realpath(relative_path->str, absolute_path);

  if (real_path == NULL) {
    free_str(relative_path);
    free(absolute_path);
    return NULL;
  }

  free_str(relative_path);

  return absolute_path;
}

string *error_response(int status_code) {
  response_t *response = new_response();

  if (response == NULL) {
    return NULL;
  }

  generate_response_status(response, status_code, CONTENT_TYPE_HTML);

  string *status_code_str = int_to_string(status_code);
  const char *status_message = get_http_status_message(status_code);

  response->body = str_set(response->body, "<html><head><title>Error</title></head><body><h1>", 49);
  str_cat(response->body, status_code_str->str, status_code_str->len);

  free_str(status_code_str);

  str_cat(response->body, "</h1><p>", 8);
  str_cat(response->body, status_message, strlen(status_message));
  str_cat(response->body, "</p></body></html>", 18);

  update_response_content_length(response);

  string *encoded_response = serialize_response(response);

  free_response(&response);

  return encoded_response;
}

string *debug_response(request_t *request) {
  response_t *response = new_response();

  if (response == NULL) {
    free_request(&request);
    return error_response(HTTP_INTERNAL_SERVER_ERROR);
  }

  generate_response_status(response, HTTP_OK, CONTENT_TYPE_HTML);

  // HTML body
  response->body = str_set(response->body, "<html><head><title>Debug</title></head><body>", 45);
  response->body = str_cat(response->body, "<p>HTTP-Methode: ", 17);
  response->body = str_cat(response->body, request->method->str, request->method->len);

  response->body = str_cat(response->body, "<br>Ressource: ", 15);
  response->body = str_cat(response->body, request->resource->str, request->resource->len);

  response->body = str_cat(response->body, "<br>HTTP-Version: ", 18);
  response->body = str_cat(response->body, request->version->str, request->version->len);
  response->body = str_cat(response->body, "</p></body></html>", 18);

  // content length
  update_response_content_length(response);

  // encode response
  string *encoded_response = serialize_response(response);

  free_response(&response);
  free_request(&request);

  return encoded_response;
}

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

string *http_server(string *raw_request) {
  request_t *decoded_request = parse_request_string(raw_request);

  if (decoded_request == NULL) {
    return error_response(HTTP_BAD_REQUEST);
  }

  // decode url-encoded resource
  string *decoded = decode_url(decoded_request->resource);

  if (decoded == NULL) {
    free_request(&decoded_request);
    return error_response(HTTP_BAD_REQUEST);
  }

  // free old resource and set new decoded resource
  str_set(decoded_request->resource, decoded->str, decoded->len);
  free_str(decoded);

  // validate request content
  if (!is_valid_request(decoded_request)) {
    free_request(&decoded_request);
    return error_response(HTTP_BAD_REQUEST);
  }

  // check if method is implemented
  if (strcmp(decoded_request->method->str, HTTP_METHOD_GET) != 0) {
    free_request(&decoded_request);
    return error_response(HTTP_NOT_IMPLEMENTED);
  }

  // return debug response if requested
  if (strcmp(decoded_request->resource->str, ROUTE_DEBUG) == 0) {
    // no cleanup needed, debug_response() will free the request
    return debug_response(decoded_request);
  }

  response_t *response = new_response();

  if (response == NULL) {
    free_request(&decoded_request);
    return error_response(HTTP_INTERNAL_SERVER_ERROR);
  }

  // create path to resource
  char *absolute_path = convert_to_absolute_path(decoded_request->resource);

  // check if resource exists and get absolute path
  if (absolute_path == NULL) {
    free_request(&decoded_request);
    free_response(&response);
    return error_response(HTTP_NOT_FOUND);
  }

  // check if path is valid
  if (!is_valid_path(absolute_path)) {
    cleanup(&decoded_request, &response, absolute_path);
    return error_response(HTTP_FORBIDDEN);
  }

  string *file_content = read_file(absolute_path);

  if (file_content == NULL) {
    int error = HTTP_NOT_FOUND;

    // check if file access was denied
    if (errno == EACCES) {
      error = HTTP_FORBIDDEN;
    }

    cleanup(&decoded_request, &response, absolute_path);
    return error_response(error);
  }

  // get mime type of file
  const char *mime_type = get_mime_type(absolute_path);

  // fill response object
  generate_response_status(response, HTTP_OK, mime_type);

  response->body = str_set(response->body, file_content->str, file_content->len);
  free_str(file_content);

  update_response_content_length(response);

  string *encoded_response = serialize_response(response);

  cleanup(&decoded_request, &response, absolute_path);

  return encoded_response;
}