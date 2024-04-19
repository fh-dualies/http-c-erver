#ifndef PSE_2024_TEST_LIB_H
#define PSE_2024_TEST_LIB_H

#include "../http-lib/httplib.h"

/**
 * @brief Expect two strings to be equal
 *
 * @param str The string to test
 * @param expected_len The expected length of the string
 * @param expected_str The expected value of the string
 */
void expect_equal(string *str, size_t expected_len, const char *expected_str);

/**
 * @brief Expect a pointer to be not NULL
 *
 * @param ptr The pointer to test
 */
void expect_not_null(void *ptr);

/**
 * @brief Expect a pointer to be NULL
 *
 * @param ptr The pointer to test
 */
void expect_null(void *ptr);

/**
 * @brief Expect a value to be true
 *
 * @param value The value to test
 */
void expect_true(int value);

/**
 * @brief Expect a value to be false
 *
 * @param value The value to test
 */
void expect_false(int value);

/**
 * @brief Print the title of a test
 *
 * @param title The title of the test
 */
void test_title(const char *title);

/**
 * @brief Print the summary of the tests
 *
 * @return The number of failed assertions
 */
int test_summary();

#endif
