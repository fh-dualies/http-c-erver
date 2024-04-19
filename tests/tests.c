#include "../lib/testing/test-lib.h"
#include "http-lib/http-lib_test.h"
#include "http_parser/http_parser_test.h"

int main() {
  run_http_parser_test();
  run_httplib_test();

  return test_summary();
}