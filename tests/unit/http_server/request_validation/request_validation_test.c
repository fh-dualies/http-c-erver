//
// Created by dennis on 23/04/24.
//

#include "request_validation_test.h"
#include "../../../../lib/testing/test-lib.h"
#include "../../../../src/http_models/http_models.h"
#include "../../../../src/http_server/request_validation/request_validation.h"

void test_request_empty() {
  test_title("Test request_empty()");

  request_t *request = new_request();
  expect_true(request_empty(request));

  str_set(request->method, "GET", 3);
  expect_true(request_empty(request));

  str_set(request->resource, "/", 1);
  expect_true(request_empty(request));

  str_set(request->version, "HTTP/1.0", 8);
  expect_false(request_empty(request));

  free_request(&request);
}

void test_supported_version() {
  test_title("Test supported_version()");

  string *version = cpy_str("HTTP/1.0", 8);
  expect_true(supported_version(version));

  str_set(version, "HTTP/1.1", 8);
  expect_true(supported_version(version));

  str_set(version, "HTTP/2.0", 8);
  expect_false(supported_version(version));

  free_str(version);
}

void test_supported_method() {
  test_title("Test supported_method()");

  string *method = cpy_str("GET", 3);
  expect_true(supported_method(method));

  str_set(method, "POST", 4);
  expect_false(supported_method(method));

  free_str(method);
}


void run_request_validation_test() {
  test_request_empty();
  test_supported_version();
  test_supported_method();
}