#include "http_server.h"
#include "../http_router/http_router.h"
#include "request_validation/request_validation.h"
#include <stdlib.h>
#include <unistd.h>

void cleanup(request_t **request, response_t **response, char *path) {
  free_request(request);
  free_response(response);
  free(path);
}

string *get_mime_type(const char *path) {
  if (path == NULL) {
    return NULL;
  }

  string *mime_type = _new_string();

  if (mime_type == NULL) {
    return NULL;
  }

  // default mime type: text/plain
  str_set(mime_type, CONTENT_TYPE_TEXT, strlen(CONTENT_TYPE_TEXT));

  /// @node strrchr() is safe here - path is a string literal
  const char *extension = strrchr(path, '.');

  if (extension == NULL) {
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_HTML) == 0) {
    str_set(mime_type, CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_CSS) == 0) {
    str_set(mime_type, CONTENT_TYPE_CSS, strlen(CONTENT_TYPE_CSS));
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_JS) == 0) {
    str_set(mime_type, CONTENT_TYPE_JS, strlen(CONTENT_TYPE_JS));
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_JPG) == 0 || strcmp(extension, EXTENSION_JPEG) == 0) {
    str_set(mime_type, CONTENT_TYPE_JPEG, strlen(CONTENT_TYPE_JPEG));
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_PNG) == 0) {
    str_set(mime_type, CONTENT_TYPE_PNG, strlen(CONTENT_TYPE_PNG));
    return mime_type;
  }

  // TODO: remove strcmp()
  if (strcmp(extension, EXTENSION_ICO) == 0) {
    str_set(mime_type, CONTENT_TYPE_ICO, strlen(CONTENT_TYPE_ICO));
    return mime_type;
  }

  return mime_type;
}

string *error_response(int status_code) {
  response_t *response = new_response();

  if (response == NULL) {
    return NULL;
  }

  /// @node strlen() is safe here - status messages is a constant defined in http_server.h
  string *content_type = cpy_str(CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
  generate_response_status(response, status_code, content_type);
  free_str(content_type);

  string *status_code_str = int_to_string(status_code);
  const char *status_message = get_http_status_message(status_code);

  response->body = str_set(response->body, "<html><head><title>Error</title></head><body><h1>", 49);
  str_cat(response->body, status_code_str->str, status_code_str->len);

  free_str(status_code_str);

  str_cat(response->body, "</h1><p>", 8);
  /// @node strlen() is safe here - status_message is a constant defined in http_server.h (via
  /// get_http_status_message())
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

  /// @node strlen() is safe here - CONTENT_TYPE_HTML is a constant defined in http_server.h
  string *content_type = cpy_str(CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
  generate_response_status(response, HTTP_OK, content_type);
  free_str(content_type);

  // HTML body
  response->body = str_set(response->body, "<html><head><title>Debug</title></head><body>", 45);
  response->body = str_cat(response->body, "<p>HTTP-Methode: ", 17);
  response->body = str_cat(response->body, request->method->str, request->method->len);

  response->body = str_cat(response->body, "<br>Ressource: ", 15);
  response->body = str_cat(response->body, request->resource->str, request->resource->len);

  response->body = str_cat(response->body, "<br>HTTP-Version: ", 18);
  response->body = str_cat(response->body, request->version->str, request->version->len);
  response->body = str_cat(response->body, "</p></body></html>", 18);

  update_response_content_length(response);

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
  case HTTP_VERSION_NOT_SUPPORTED:
    return STATUS_MESSAGE_VERSION_NOT_SUPPORTED;
  default:
    return STATUS_MESSAGE_UNKNOWN;
  }
}

string *http_server(string *raw_request) {
  request_t *decoded_request = parse_request_string(raw_request);

  if (decoded_request == NULL) {
    return error_response(HTTP_BAD_REQUEST);
  }

  string *decoded = decode_url(decoded_request->resource);

  if (decoded == NULL) {
    free_request(&decoded_request);
    return error_response(HTTP_BAD_REQUEST);
  }

  // free old resource and set new decoded resource
  str_set(decoded_request->resource, decoded->str, decoded->len);
  free_str(decoded);

  if (request_empty(decoded_request)) {
    free_request(&decoded_request);
    return error_response(HTTP_BAD_REQUEST);
  }

  if (!supported_version(decoded_request->version)) {
    free_request(&decoded_request);
    return error_response(HTTP_VERSION_NOT_SUPPORTED);
  }

  if (!supported_method(decoded_request->method)) {
    free_request(&decoded_request);
    return error_response(HTTP_NOT_IMPLEMENTED);
  }

  return route_request(decoded_request);
}