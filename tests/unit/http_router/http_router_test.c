#include "http_router_test.h"
#include "../../../lib/testing/unit/test-lib.h"
#include "../../../src/http_router/http_router.h"

void test_get_host_directory() {
  test_title("Test get_host_directory()");

  request_t *request = new_request();

  string *host_directory = get_host_directory(request);
  expect_equal(host_directory, 8, "/default");
  free_str(host_directory);

  str_set(request->host, "EXTERN", 6);
  host_directory = get_host_directory(request);
  expect_equal(host_directory, 7, "/extern");
  free_str(host_directory);

  str_set(request->host, "INTERN", 6);
  host_directory = get_host_directory(request);
  expect_equal(host_directory, 7, "/intern");
  free_str(host_directory);

  str_set(request->host, "UNKNOWN", 7);
  host_directory = get_host_directory(request);
  expect_equal(host_directory, 8, "/default");
  free_str(host_directory);

  free_request(&request);
}

void test_valid_path() {
  test_title("Test valid_path()");

  expect_false(valid_path(NULL, NULL));

  string *path = str_cpy("/home/user/src/htdocs/index.html", 32);
  string *host_extension = str_cpy("/default", 8);
  expect_false(valid_path(path, host_extension));

  str_set(path, "/home/user/src/htdocs/intern/index.html", 39);
  expect_false(valid_path(path, host_extension));

  str_set(path, "home/user/src/htdocs/default/index.html", 39);
  expect_false(valid_path(path, host_extension));

  str_set(path, "/home/user/src/htdocs/default/index.html", 40);
  expect_true(valid_path(path, host_extension));

  free_str(path);
  free_str(host_extension);
}

void run_http_router_test() {
  test_get_host_directory();
  test_valid_path();
}