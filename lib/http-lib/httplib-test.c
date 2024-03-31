#include "httplib.h"
#include "../testing/test-lib.h"
#include <stdio.h>

void test_url_decode() {
  printf("\nTest URL decode\n");

  string *str = cpy_str("Hello+World%21", 14);

  string *decoded = url_decode(str);

  expect_equal(decoded, 12, "Hello World!");

  free_str(str);
  free_str(decoded);
}

void test_url_encode() {
  printf("\nTest URL encode\n");

  string *str = cpy_str("Hello World!", 12);

  string *encoded = url_encode(str);

  expect_equal(encoded, 14, "Hello+World%21");

  free_str(str);
  free_str(encoded);
}

void test_str_cat() {
  printf("\nTest str_cat\n");

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

// Praktikum 1 "str_cat"-Task example
int main() {
  test_str_cat();
  test_url_decode();
  test_url_encode();

  return 0;
}