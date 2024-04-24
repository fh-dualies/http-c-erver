#ifndef STRING_LIB_H
#define STRING_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
  size_t len;
  char *str;
} typedef string;

/**
 * @brief Exit the program with an error message
 *
 * @param function_name The name of the function that caused the error
 * @param reason The reason for the error
 */
void *exit_err(const char *function_name, const char *reason);

/**
 * @brief Concatenate a string to another string
 * @warning The destination will be reallocated
 * @warning This function will add a null terminator to the end of the string
 *
 * Returns NULL if the destination string is NULL.
 * Returns the destination string if the source string is NULL or len is 0.
 * Exits with code 1 if the memory allocation fails.
 *
 * @param dest The destination string
 * @param src The source string
 * @param len The length of the source string
 * @return The destination string
 */
string *str_cat(string *dest, const char *src, size_t len);

/**
 * @brief Set a string to another string by freeing the destination string and allocating memory for
 * the source string
 * @warning The destination will be reallocated
 * @warning This function will add a null terminator to the end of the string
 *
 * Returns NULL if the destination string is NULL.
 * Returns the destination string if the source string is NULL or len is 0.
 * Exits with code 1 if the memory allocation fails.
 *
 * @param dest
 * @param src
 * @param len
 * @return
 */
string *str_set(string *dest, const char *src, size_t len);

/**
 * @brief Compare two strings
 *
 * Returns 0 if the strings are equal.
 * Returns -1 if the first string is less than the second string.
 * Returns 1 if the first string is greater than the second string.
 *
 * @param str1 The first string
 * @param str2 The second string
 * @return The comparison result
 */
int str_cmp(string *str1, const char *str2);

/**
 * @brief Convert a string to lower case
 *
 * Returns if the input string is NULL.
 *
 * @param input The string to convert
 */
void str_to_lower(string *input);

/**
 * @brief Create a new string
 * @warning This function will add a null terminator to the end of the string
 *
 * Exits with code 1 if the memory allocation of the string struct fails or if the memory allocation
 * of the string fails.
 *
 * @return The new string
 */
string *_new_string();

/**
 * @brief Print a string
 *
 * @param str The string to print
 */
void print_string(string *str);

/**
 * @brief Copy a string
 * @warning The destination will be reallocated
 * @warning This function will add a null terminator to the end of the string
 *
 * Exits with code 1 if the memory allocation of the string struct fails or if the memory allocation
 * of the string fails.
 *
 * @param src The source string
 * @param len The length of the source string
 * @return The copied string
 */
string *cpy_str(const char *src, size_t len);

/**
 * @brief Free a string
 *
 * Returns if the string is NULL.
 *
 * @param str The string to free
 */
void free_str(string *str);

/**
 * @brief Get the length of a string
 *
 * @param str The string
 * @return The length of the string
 */
size_t get_length(string *str);

/**
 * @brief Get the value of a string
 *
 * @param str The string
 * @return The value of the string
 */
char *get_char_str(string *str);

/**
 * @brief Convert a size_t to a string
 * @waring The return value must be freed after use
 *
 * Exits with code 1 if the memory allocation fails.
 *
 * @param num The size_t
 * @return The string
 */
string *size_t_to_string(size_t num);

/**
 * @brief Convert an int to a string
 * @waring The return value must be freed after use
 *
 * Exits with code 1 if the memory allocation fails.
 *
 * @param num The int
 * @return The string
 */
string *int_to_string(int num);

#endif