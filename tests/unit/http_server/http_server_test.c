#include "http_server_test.h"
#include "../../../lib/testing/unit/test-lib.h"
#include "../../../src/http_server/http_server.h"

void run_cleanup_test() {
  request_t *request = new_request();
  response_t *response = new_response();
  char *path = (char *)malloc(sizeof(char) * 10);

  cleanup(&request, &response, path);

  expect_null(request);
  expect_null(response);
}

void test_get_mime_type() {
  test_title("Test get_mime_type()");

  string *mime_type = get_mime_type("index");
  expect_equal(mime_type, strlen(CONTENT_TYPE_TEXT), CONTENT_TYPE_TEXT);

  mime_type = get_mime_type("index.html");
  expect_equal(mime_type, strlen(CONTENT_TYPE_HTML), CONTENT_TYPE_HTML);

  mime_type = get_mime_type("index.css");
  expect_equal(mime_type, strlen(CONTENT_TYPE_CSS), CONTENT_TYPE_CSS);

  mime_type = get_mime_type("index.js");
  expect_equal(mime_type, strlen(CONTENT_TYPE_JS), CONTENT_TYPE_JS);

  mime_type = get_mime_type("index.jpg");
  expect_equal(mime_type, strlen(CONTENT_TYPE_JPEG), CONTENT_TYPE_JPEG);

  mime_type = get_mime_type("index.jpeg");
  expect_equal(mime_type, strlen(CONTENT_TYPE_JPEG), CONTENT_TYPE_JPEG);

  mime_type = get_mime_type("index.png");
  expect_equal(mime_type, strlen(CONTENT_TYPE_PNG), CONTENT_TYPE_PNG);

  mime_type = get_mime_type("index.ico");
  expect_equal(mime_type, strlen(CONTENT_TYPE_ICO), CONTENT_TYPE_ICO);

  free_str(mime_type);
}

void test_error_response() {
  test_title("Test error_response()");

  string *response = error_response(404);

  expect_equal(
      response, 188,
      "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 87\r\nServer: LLDM/0.1 "
      "HTTP Server\r\n\r\n<html><head><title>Error</title></head><body><h1>404</h1><p>Not "
      "Found</p></body></html>\r\n");

  free_str(response);
}

void test_debug_response() {
  test_title("Test debug_response()");

  request_t *request = new_request();
  str_set(request->method, HTTP_METHOD_GET, strlen(HTTP_METHOD_GET));
  str_set(request->resource, "/", 1);
  str_set(request->version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));

  string *response = debug_response(request);

  expect_equal(
      response, 220,
      "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 125\r\nServer: LLDM/0.1 "
      "HTTP Server\r\n\r\n<html><head><title>Debug</title></head><body><p>HTTP-Methode: "
      "GET<br>Ressource: /<br>HTTP-Version: HTTP/1.1</p></body></html>\r\n");

  free_str(response);
}

void test_get_http_status_message() {
  test_title("Test get_http_status_message()");

  string *status_message = _new_string();

  str_set(status_message, get_http_status_message(HTTP_OK), strlen(STATUS_MESSAGE_OK));
  expect_equal(status_message, strlen(STATUS_MESSAGE_OK), STATUS_MESSAGE_OK);

  str_set(status_message, get_http_status_message(HTTP_BAD_REQUEST),
          strlen(STATUS_MESSAGE_BAD_REQUEST));
  expect_equal(status_message, strlen(STATUS_MESSAGE_BAD_REQUEST), STATUS_MESSAGE_BAD_REQUEST);

  str_set(status_message, get_http_status_message(HTTP_FORBIDDEN),
          strlen(STATUS_MESSAGE_FORBIDDEN));
  expect_equal(status_message, strlen(STATUS_MESSAGE_FORBIDDEN), STATUS_MESSAGE_FORBIDDEN);

  str_set(status_message, get_http_status_message(HTTP_NOT_FOUND),
          strlen(STATUS_MESSAGE_NOT_FOUND));
  expect_equal(status_message, strlen(STATUS_MESSAGE_NOT_FOUND), STATUS_MESSAGE_NOT_FOUND);

  str_set(status_message, get_http_status_message(HTTP_INTERNAL_SERVER_ERROR),
          strlen(STATUS_MESSAGE_INTERNAL_SERVER_ERROR));
  expect_equal(status_message, strlen(STATUS_MESSAGE_INTERNAL_SERVER_ERROR),
               STATUS_MESSAGE_INTERNAL_SERVER_ERROR);

  str_set(status_message, get_http_status_message(HTTP_NOT_IMPLEMENTED),
          strlen(STATUS_MESSAGE_NOT_IMPLEMENTED));
  expect_equal(status_message, strlen(STATUS_MESSAGE_NOT_IMPLEMENTED),
               STATUS_MESSAGE_NOT_IMPLEMENTED);

  str_set(status_message, get_http_status_message(HTTP_VERSION_NOT_SUPPORTED),
          strlen(STATUS_MESSAGE_VERSION_NOT_SUPPORTED));
  expect_equal(status_message, strlen(STATUS_MESSAGE_VERSION_NOT_SUPPORTED),
               STATUS_MESSAGE_VERSION_NOT_SUPPORTED);
  str_set(status_message, get_http_status_message(666), strlen(STATUS_MESSAGE_UNKNOWN));
  expect_equal(status_message, strlen(STATUS_MESSAGE_UNKNOWN), STATUS_MESSAGE_UNKNOWN);

  free_str(status_message);
}

void run_http_server_test() {
  run_cleanup_test();
  test_get_mime_type();
  test_error_response();
  test_debug_response();
  test_get_http_status_message();
}