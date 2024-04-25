#include "http-lib_test.h"
#include "../../../lib/string_lib/string_lib.h"
#include "../../../lib/testing/unit/test-lib.h"

void test_str_cat() {
  test_title("Test str_cat()");

  string *str1 = str_cpy("Hello, ", 7);
  string *str2 = str_cpy("world!", 6);
  string *str_empty = str_cpy("", 0);

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

void test_str_set() {
  test_title("Test str_set()");

  string *string = _new_string();

  str_set(string, "Hello, world!", 13);
  expect_equal(string, 13, "Hello, world!");

  str_set(string, "world!", 6);
  expect_equal(string, 6, "world!");

  free_str(string);
}

void test_new_string() {
  test_title("Test new_string()");

  string *string = _new_string();
  expect_equal(string, 0, "");
  expect_not_null(&string->len);

  free_str(string);
}

void test_cpy_str() {
  test_title("Test str_cpy()");

  string *string = str_cpy("Hello, world!", 13);
  expect_equal(string, 13, "Hello, world!");

  free_str(string);
}

void test_get_length() {
  test_title("Test get_length()");

  string *str = str_cpy("Hello, world!", 13);
  string *len = size_t_to_string(get_length(str));

  expect_equal(len, 2, "13");

  free_str(str);
  free_str(len);
}

void test_get_char_str() {
  test_title("Test get_char_str()");

  string *str = str_cpy("Hello, world!", 13);
  string *str2 = _new_string();

  str_set(str2, get_char_str(str), get_length(str));

  expect_equal(str2, 13, "Hello, world!");

  free_str(str);
  free_str(str2);
}

void test_int_to_string() {
  test_title("Test int_to_string()");

  string *str = int_to_string(123);
  expect_equal(str, 3, "123");

  free_str(str);
}

void test_size_t_to_string() {
  test_title("Test size_t_to_string()");

  string *str = size_t_to_string(123);
  expect_equal(str, 3, "123");

  free_str(str);
}

void run_httplib_test() {
  test_str_cat();
  test_str_set();
  test_new_string();
  test_cpy_str();
  test_get_length();
  test_get_char_str();
  test_int_to_string();
  test_size_t_to_string();
}