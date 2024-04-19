#include "http_parser_test.h"
#include "../../lib/testing/test-lib.h"
#include "../../src/http_parser/http_parser.h"

void test_url_decode() {
  test_title("Test URL decode");

  string *str = cpy_str("Hello+World%21", 14);

  string *decoded = url_decode(str);

  expect_equal(decoded, 12, "Hello World!");

  free_str(str);
  free_str(decoded);
}

void test_url_encode() {
  test_title("Test URL encode");

  string *str = cpy_str("Hello World!", 12);

  string *encoded = url_encode(str);

  expect_equal(encoded, 14, "Hello+World%21");

  free_str(str);
  free_str(encoded);
}

void run_http_parser_test() {
  test_url_decode();
  test_url_encode();
}