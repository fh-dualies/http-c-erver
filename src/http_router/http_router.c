#include "http_router.h"
#include "../../lib/file_lib/file_lib.h"
#include "../../main.h"
#include "../http_server/http_server.h"
#include <errno.h>
#include <limits.h>

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

  // add document root to relative path
  str_cat(relative_path, DOCUMENT_ROOT, strlen(DOCUMENT_ROOT));
  // add host extension to relative path
  str_cat(relative_path, get_char_str(host_extension), get_length(host_extension));
  // add resource to relative path
  str_cat(relative_path, get_char_str(resource), get_length(resource));

  char *real_path = realpath(get_char_str(relative_path), absolute_path);

  if (real_path == NULL) {
    free_str(relative_path);
    free(absolute_path);
    return NULL;
  }

  free_str(relative_path);

  /// @node strlen() is safe to use here - absolute_path is a default char array
  string *absolute = str_cpy(absolute_path, strlen(absolute_path));
  free(absolute_path);
  return absolute;
}

bool valid_path(string *path, string *host_extension) {
  if (path == NULL || host_extension == NULL) {
    return false;
  }

  if (get_length(path) == 0) {
    return false;
  }

  if (path->str[0] != '/') {
    return false;
  }

  string *temp = _new_string();

  if (temp == NULL) {
    return false;
  }

  str_set(temp, DOCUMENT_ROOT, strlen(DOCUMENT_ROOT));

  // should contain DOCUMENT_ROOT (mainly prevents access to other directories)
  if (str_str(path, temp) == NULL) {
    free_str(temp);
    return false;
  }

  str_set(temp, "..", 2);

  if (str_str(path, temp) != NULL) {
    free_str(temp);
    return false;
  }

  free_str(temp);

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

  string *mime_type = get_mime_type(get_char_str(path));

  generate_response_status(response, HTTP_OK, mime_type);
  free_str(mime_type);

  response->body = str_set(response->body, get_char_str(file_content), get_length(file_content));
  free_str(file_content);

  update_response_content_length(response);

  string *encoded_response = serialize_response(response);

  free_response(&response);
  return encoded_response;
}

string *route_request(request_t *request) {
  if (str_cmp(request->resource, ROUTE_DEBUG) == 0) {
    // no cleanup needed, debug_response() will free the request
    return debug_response(request);
  }

  string *host = request->host;
  string *path_extension = NULL;

  if (host == NULL) {
    path_extension = str_cpy(ROUTE_DEFAULT_HOST, strlen(ROUTE_DEFAULT_HOST));
  } else {
    // Split host header on ':'
    string *new_host = _new_string();

    for (int i = 0; i < get_length(host); i++) {
      if (host->str[i] == ':') {
        break;
      }

      str_cat(new_host, &host->str[i], 1);
    }

    str_set(host, get_char_str(new_host), get_length(new_host));
    free_str(new_host);
  }

  if (str_cmp(host, HOST_EXTERN) == 0) {
    path_extension = str_cpy(ROUTE_EXTERN_HOST, strlen(ROUTE_EXTERN_HOST));
  }

  if (str_cmp(host, HOST_INTERN) == 0) {
    free_request(&request);
    free_str(path_extension);
    return error_response(HTTP_UNAUTHORIZED);
  }

  if (path_extension == NULL) {
    path_extension = str_cpy(ROUTE_DEFAULT_HOST, strlen(ROUTE_DEFAULT_HOST));
  }

  string *path = convert_to_absolute_path(request->resource, path_extension);

  if (path == NULL) {
    free_request(&request);
    free_str(path_extension);
    return error_response(HTTP_NOT_FOUND);
  }

  if (!valid_path(path, path_extension)) {
    free_request(&request);
    free_str(path);
    free_str(path_extension);
    return error_response(HTTP_FORBIDDEN);
  }

  string *response = serve_file(path);

  free_str(path);
  free_str(path_extension);
  free_request(&request);

  return response;
}