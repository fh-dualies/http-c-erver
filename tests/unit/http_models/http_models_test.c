#include "http_models_test.h"
#include "../../../lib/testing/test-lib.h"
#include "../../../src/http_models/http_models.h"
#include "../../../src/http_server/http_server.h"

void test_new_request() {
  test_title("Test new_request()");

  request_t *request = new_request();

  expect_not_null(request);

  free_request(&request);
}

void test_new_response() {
  test_title("Test new_response()");

  response_t *response = new_response();

  expect_not_null(response);

  free_response(&response);
}

void test_free_request() {
  test_title("Test free_request()");

  request_t *request = new_request();
  free_request(&request);

  expect_null(request);
}

void test_free_response() {
  test_title("Test free_response");

  response_t *response = new_response();
  free_response(&response);

  expect_null(response);
}

void test_update_response_content_length() {
  test_title("Test update_response_content_length()");

  response_t *response = new_response();
  str_cat(response->body, "Test body content", 17);

  update_response_content_length(response);

  expect_equal(response->content_length, 2, "17");

  free_response(&response);
}

void test_generate_response_status() {
  test_title("Test generate_response_status()");

  response_t *response = new_response();
  generate_response_status(response, HTTP_OK, CONTENT_TYPE_HTML);

  expect_equal(response->version, strlen(HTTP_VERSION_1_1), HTTP_VERSION_1_1);
  expect_equal(response->status_code, 3, "200");
  expect_equal(response->status_message, strlen(STATUS_MESSAGE_OK), STATUS_MESSAGE_OK);
  expect_equal(response->content_type, strlen(CONTENT_TYPE_HTML), CONTENT_TYPE_HTML);
  expect_equal(response->server, strlen(SERVER_SIGNATURE), SERVER_SIGNATURE);

  free_response(&response);
}

void test_add_response_string_header() {
  test_title("Test add_response_string_header()");

  string *raw_string = _new_string();
  string *value = _new_string();

  str_cat(value, "Test value", 10);

  add_response_string_header(raw_string, "Test-Header: ", value);

  expect_equal(raw_string, 25, "Test-Header: Test value\r\n");

  free_str(raw_string);
  free_str(value);
}

void test_generate_response_status_line() {
  test_title("Test generate_response_status_line()");

  string *raw_string = _new_string();
  string *version = _new_string();
  string *status_code = _new_string();
  string *status_message = _new_string();

  str_cat(version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));
  str_cat(status_code, "200", 3);
  str_cat(status_message, STATUS_MESSAGE_OK, strlen(STATUS_MESSAGE_OK));

  generate_response_status_line(raw_string, version, status_code, status_message);

  expect_equal(raw_string, 17, "HTTP/1.1 200 OK\r\n");

  free_str(raw_string);
  free_str(version);
  free_str(status_code);
  free_str(status_message);
}

void run_http_models_test() {
  test_new_request();
  test_new_response();
  test_free_request();
  test_free_response();
  test_update_response_content_length();
  test_generate_response_status();
  test_add_response_string_header();
  test_generate_response_status_line();
}