#include "httplib.h"
#include <stdio.h>

// Praktikum 1 "str_cat"-Task example
int main() {
  string *str1 = cpy_str("Hello, ", 7);
  string *str2 = cpy_str("world!", 6);
  string *str_empty = cpy_str("", 0);

  str_cat(str1, get_char_str(str2), get_length(str2));
  str_cat(str2, get_char_str(str_empty), get_length(str_empty));
  str_cat(str_empty, get_char_str(str2), get_length(str2));

  print_string(str1);
  putchar('\n');
  print_string(str2);
  putchar('\n');
  print_string(str_empty);
  putchar('\n');

  free_str(str1);
  free_str(str2);

  return 0;
}