#ifndef HTTPLIB_H
#define HTTPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
  size_t len;
  char *str;
} typedef string;

/// @brief Concatenate a string to another string
/// @param dest The destination string
/// @param src The source string
/// @param len The length of the source string
/// @return The destination string
string *str_cat(string *dest, const char *src, size_t len);

/// @brief Set a string
/// @param dest The destination string
/// @param src The source string
/// @param len The length of the source string
/// @return The destination string
string *str_set(string *dest, const char *src, size_t len);

/// @brief Create a new string
string *_new_string();

/// @brief Print a string
/// @param str The string to print
void print_string(string *str);

/// @brief Copy a string
/// @param src The source string
/// @param len The length of the source string
/// @return The copied string
string *cpy_str(const char *src, size_t len);

/// @brief Free a string
/// @param str The string to free
void free_str(string *str);

/// @brief Get the length of a string
/// @param str The string
/// @return The length of the string
size_t get_length(string *str);

/// @brief Get the value of a string
/// @param str The string
/// @return The value of the string
char *get_char_str(string *str);

/// @brief Read a file
/// @param path The path to the file
/// @return The content of the file
string *read_file(char *path);

/// @brief Convert an size_t to a string
/// @param num The size_t
/// @return The string
string *size_t_to_string(size_t num);

/// @brief Convert an int to a string
/// @param num The int
/// @return The string
string *int_to_string(int num);

#endif
