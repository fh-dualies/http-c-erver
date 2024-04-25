#include "http_parser_test.h"
#include "../../../lib/testing/unit/test-lib.h"
#include "../../../src/http_parser/http_parser.h"
#include "../../../src/http_server/http_server.h"

void test_parse_request_string() {
  test_title("Test parse_request_string()");

  string *raw_request = str_cpy("GET / HTTP/1.1\nHost: localhost:8080\n\n", 36);

  request_t *request = parse_request_string(raw_request);

  expect_equal(request->method, strlen(HTTP_METHOD_GET), HTTP_METHOD_GET);
  expect_equal(request->resource, 1, "/");
  expect_equal(request->version, strlen(HTTP_VERSION_1_1), HTTP_VERSION_1_1);

  free_request(&request);
  free_str(raw_request);
}

void test_serialize_response() {
  test_title("Test serialize_response()");

  response_t *response = new_response();
  str_set(response->version, HTTP_VERSION_1_1, strlen(HTTP_VERSION_1_1));
  str_set(response->status_code, "200", 3);
  str_set(response->status_message, STATUS_MESSAGE_OK, strlen(STATUS_MESSAGE_OK));
  str_set(response->content_type, CONTENT_TYPE_HTML, strlen(CONTENT_TYPE_HTML));
  str_set(response->content_length, "21", 2);
  str_set(response->server, SERVER_SIGNATURE, strlen(SERVER_SIGNATURE));
  str_set(response->body, "<h1>Hello World!</h1>", 21);

  string *serialized = serialize_response(response);

  expect_equal(serialized, 115,
               "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 21\r\nServer: "
               "LLDM/0.1 HTTP Server\r\n\r\n<h1>Hello World!</h1>");

  free_response(&response);
  free_str(serialized);
}

void test_decode_url() {
  test_title("Test decode_url()");

  string *str = str_cpy("Hello+World%21", 14);

  string *decoded = decode_url(str);

  expect_equal(decoded, 12, "Hello World!");

  free_str(str);
  free_str(decoded);
}

void test_encode_url() {
  test_title("Test url_encode()");

  string *str = str_cpy("Hello World!", 12);

  string *encoded = encode_url(str);

  expect_equal(encoded, 14, "Hello+World%21");

  free_str(str);
  free_str(encoded);
}

void run_http_parser_test() {
  test_parse_request_string();
  test_serialize_response();
  test_decode_url();
  test_encode_url();
}