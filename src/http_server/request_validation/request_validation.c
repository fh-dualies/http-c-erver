//
// Created by dennis on 23/04/24.
//

#include "request_validation.h"
#include "../http_server.h"

bool request_empty(request_t *request) {
  return request->method == NULL || request->resource == NULL || request->version == NULL ||
         request->method->len == 0 || request->resource->len == 0 || request->version->len == 0;
}

bool supported_version(string *version) {
  return strcmp(version->str, HTTP_VERSION_1_0) == 0 || strcmp(version->str, HTTP_VERSION_1_1) == 0;
}

bool supported_method(string *method) {
  return strcmp(method->str, HTTP_METHOD_GET) == 0;
}