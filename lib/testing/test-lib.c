#include "test-lib.h"

// Assertion counters
int assertions = 0;
int failed_assertions = 0;

void error_color() { printf("\033[31m"); }

void success_color() { printf("\033[32m"); }

void title_color() { printf("\033[35m"); }

void reset_color() { printf("\033[0m"); }

void expect_equal(string *str, size_t expected_len, const char *expected_str) {
  assertions++;

  string *expected = cpy_str(expected_str, expected_len);

  // Check if the lengths are equal
  if (str->len != expected->len) {
    error_color();
    printf("Expected length %zu, got %zu\n", expected->len, str->len);
    reset_color();

    free_str(expected);
    failed_assertions++;
    return;
  }

  // Check if the strings are equal
  for (size_t i = 0; i < str->len; i++) {
    if (str->str[i] != expected->str[i]) {
      error_color();
      printf("Expected:");
      print_string(expected);
      printf("\nGot:");
      print_string(str);
      reset_color();

      free_str(expected);
      failed_assertions++;
      return;
    }
  }

  // Print success message
  success_color();
  print_string(str);
  printf(" == ");
  print_string(expected);
  printf("\n");
  reset_color();

  free_str(expected);
}

void test_title(const char *title) {
  title_color();
  printf("\n");
  printf("========================================\n");
  printf("%s\n", title);
  printf("========================================\n");
  printf("\n");
  reset_color();
}

int test_summary() {
  puts("\nTest suite executed successfully!\n");

  // Check if all assertions passed
  if (failed_assertions == 0) {
    success_color();
    printf("========================================\n");
    printf("All %d assertions passed\n", assertions);
    printf("========================================\n");
    reset_color();
    return 0;
  }

  // Print error message
  error_color();
  printf("========================================\n");
  printf("%d/%d assertions failed\n", failed_assertions, assertions);
  printf("========================================\n");
  reset_color();
  return 1;
}