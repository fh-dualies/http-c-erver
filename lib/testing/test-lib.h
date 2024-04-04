#ifndef PSE_2024_TEST_LIB_H
#define PSE_2024_TEST_LIB_H

#include "../http-lib/httplib.h"

/// @brief Expect two strings to be equal
/// @param str The string to test
/// @param expected_len The expected length of the string
/// @param expected_str The expected value of the string
void expect_equal(string *str, size_t expected_len, const char *expected_str);

/// @brief Print the title of a test
void test_title(const char *title);

/// @brief Print the summary of the tests
int test_summary();

#endif
