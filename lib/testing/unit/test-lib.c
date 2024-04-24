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

void expect_not_null(void *ptr) {
  assertions++;

  if (ptr == NULL) {
    error_color();
    printf("Expected not NULL, got NULL\n");
    reset_color();
    failed_assertions++;

    return;
  }

  success_color();
  printf("Not NULL\n");
  reset_color();
}

void expect_null(void *ptr) {
  assertions++;

  if (ptr != NULL) {
    error_color();
    printf("Expected not NULL, got NULL\n");
    reset_color();
    failed_assertions++;

    return;
  }

  success_color();
  printf("Not NULL\n");
  reset_color();
}

void expect_true(int value) {
  assertions++;

  if (!value) {
    error_color();
    printf("Expected true, got false\n");
    reset_color();
    failed_assertions++;

    return;
  }

  success_color();
  printf("true\n");
  reset_color();
}

void expect_false(int value) {
  assertions++;

  if (value) {
    error_color();
    printf("Expected false, got true\n");
    reset_color();
    failed_assertions++;

    return;
  }

  success_color();
  printf("false\n");
  reset_color();
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