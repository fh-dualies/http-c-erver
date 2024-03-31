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

/// @brief URL decode a string
/// @param str The string to decode
/// @return The decoded string
string *url_decode(string *str);

/// @brief URL encode a string
/// @param str The string to encode
/// @return The encoded string
string *url_encode(string *str);
#endif
