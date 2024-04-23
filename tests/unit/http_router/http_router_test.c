//
// Created by dennis on 23/04/24.
//

#include "http_router_test.h"
#include "../../../lib/testing/test-lib.h"
#include "../../../src/http_router/http_router.h"

void test_get_host_folder() {
  test_title("Test get_host_folder()");

  request_t *request = new_request();

  string *host_folder = get_host_folder(request);
  expect_equal(host_folder, 8, "/default");
  free_str(host_folder);

  str_set(request->host, "EXTERN", 6);
  host_folder = get_host_folder(request);
  expect_equal(host_folder, 7, "/extern");
  free_str(host_folder);

  str_set(request->host, "INTERN", 6);
  host_folder = get_host_folder(request);
  expect_equal(host_folder, 7, "/intern");
  free_str(host_folder);

  str_set(request->host, "UNKNOWN", 7);
  host_folder = get_host_folder(request);
  expect_equal(host_folder, 8, "/default");
  free_str(host_folder);

  free_request(&request);
}

void test_valid_path() {
  test_title("Test valid_path()");

  expect_false(valid_path(NULL, NULL));

  string *path = cpy_str("/home/user/src/htdocs/index.html", 32);
  string *host_extension = cpy_str("/default", 8);
  expect_false(valid_path(path, host_extension));

  str_set(path, "/home/user/src/htdocs/intern/index.html", 41);
  expect_false(valid_path(path, host_extension));

  str_set(path, "home/user/src/htdocs/default/index.html", 40);
  expect_false(valid_path(path, host_extension));

  str_set(path, "/home/user/src/htdocs/default/index.html", 42);
  expect_true(valid_path(path, host_extension));

  free_str(path);
  free_str(host_extension);
}

void run_http_router_test() {
  test_get_host_folder();
  test_valid_path();
}