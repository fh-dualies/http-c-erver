#include "http-lib_test.h"
#include "../../lib/http-lib/httplib.h"
#include "../../lib/testing/test-lib.h"

void test_str_cat() {
  test_title("Test str_cat");

  string *str1 = cpy_str("Hello, ", 7);
  string *str2 = cpy_str("world!", 6);
  string *str_empty = cpy_str("", 0);

  str_cat(str1, get_char_str(str2), get_length(str2));
  str_cat(str2, get_char_str(str_empty), get_length(str_empty));
  str_cat(str_empty, get_char_str(str2), get_length(str2));

  expect_equal(str1, 13, "Hello, world!");
  expect_equal(str2, 6, "world!");
  expect_equal(str_empty, 6, "world!");

  free_str(str1);
  free_str(str2);
  free_str(str_empty);
}

void run_httplib_test() { test_str_cat(); }