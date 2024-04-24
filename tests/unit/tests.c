#include "../../lib/testing/unit/test-lib.h"
#include "http-lib/http-lib_test.h"
#include "http_models/http_models_test.h"
#include "http_parser/http_parser_test.h"
#include "http_router/http_router_test.h"
#include "http_server/http_server_test.h"
#include "http_server/request_validation/request_validation_test.h"

int main() {
  run_httplib_test();
  run_http_models_test();
  run_http_parser_test();
  run_http_router_test();
  run_http_server_test();
  run_request_validation_test();

  return test_summary();
}