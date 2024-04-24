#include "http_router.h"
#include "../http_server/http_server.h"
#include <errno.h>
#include <limits.h>

string *get_host_directory(request_t *request) {
  string *host = request->host;

  if (host == NULL) {
    /// @node strlen() is safe to use here - ROUTE_DEFAULT_HOST is a constant defined in
    /// http_router.h
    return cpy_str(ROUTE_DEFAULT_HOST, strlen(ROUTE_DEFAULT_HOST));
  }

  // TODO: remove strcmp()
  if (strcmp(host->str, "EXTERN") == 0) {
    return cpy_str("/extern", 7);
  }

  // TODO: remove strcmp()
  if (strcmp(host->str, "INTERN") == 0) {
    return cpy_str("/intern", 7);
  }

  // if host does not match any of the above, return default folder
  return cpy_str(ROUTE_DEFAULT_HOST, strlen(ROUTE_DEFAULT_HOST));
}

string *convert_to_absolute_path(string *resource, string *host_extension) {
  if (resource == NULL) {
    return NULL;
  }

  string *relative_path = _new_string();
  char *absolute_path = calloc(PATH_MAX, 1);

  if (absolute_path == NULL) {
    free_str(relative_path);
    return NULL;
  }

  /// @node strlen() is safe to use here - DOCUMENT_ROOT is a constant defined in http_router.h
  // add document root to relative path
  str_cat(relative_path, DOCUMENT_ROOT, strlen(DOCUMENT_ROOT));
  // add host extension to relative path
  str_cat(relative_path, host_extension->str, host_extension->len);
  // add resource to relative path
  str_cat(relative_path, resource->str, resource->len);

  // TODO: is realpath() safe to use here?
  char *real_path = realpath(relative_path->str, absolute_path);

  if (real_path == NULL) {
    free_str(relative_path);
    free(absolute_path);
    return NULL;
  }

  free_str(relative_path);

  /// @node strlen() is safe to use here - absolute_path is a default char array
  return cpy_str(absolute_path, strlen(absolute_path));
}

bool valid_path(string *path, string *host_extension) {
  if (path == NULL || host_extension == NULL) {
    return false;
  }

  if (path->len == 0) {
    return false;
  }

  if (path->str[0] != '/') {
    return false;
  }

  // TODO: remove strstr()
  // should contain DOCUMENT_ROOT (mainly prevents access to other directories)
  if (strstr(path->str, DOCUMENT_ROOT) == NULL) {
    return false;
  }

  // TODO: remove strstr()
  // should contain host extension
  if (strstr(path->str, host_extension->str) == NULL) {
    return false;
  }

  // TODO: remove strstr()
  if (strstr(path->str, "..") != NULL) {
    return false;
  }

  return true;
}

string *serve_file(string *path) {
  if (path == NULL) {
    return error_response(HTTP_NOT_FOUND);
  }

  string *file_content = read_file(path);

  if (file_content == NULL) {
    int error = HTTP_NOT_FOUND;

    if (errno == EACCES) {
      error = HTTP_FORBIDDEN;
    }

    free_str(file_content);
    return error_response(error);
  }

  response_t *response = new_response();

  if (response == NULL) {
    free_str(file_content);
    return error_response(HTTP_INTERNAL_SERVER_ERROR);
  }

  // TODO: improve this
  const char *mime_type_char = get_mime_type(path->str);
  string *mime_type = cpy_str(mime_type_char, strlen(mime_type_char));

  generate_response_status(response, HTTP_OK, mime_type);
  free_str(mime_type);

  response->body = str_set(response->body, file_content->str, file_content->len);
  free_str(file_content);

  update_response_content_length(response);

  string *encoded_response = serialize_response(response);

  free_response(&response);
  return encoded_response;
}

string *route_request(request_t *request) {
  // TODO: remove strcmp()
  if (strcmp(request->resource->str, ROUTE_DEBUG) == 0) {
    // no cleanup needed, debug_response() will free the request
    return debug_response(request);
  }

  string *host_extension = get_host_directory(request);

  string *path = convert_to_absolute_path(request->resource, host_extension);

  if (path == NULL) {
    free_request(&request);
    free_str(host_extension);
    return error_response(HTTP_NOT_FOUND);
  }

  if (!valid_path(path, host_extension)) {
    free_request(&request);
    free_str(path);
    free_str(host_extension);
    return error_response(HTTP_FORBIDDEN);
  }

  string *response = serve_file(path);

  free_str(path);
  free_str(host_extension);
  free_request(&request);

  return response;
}