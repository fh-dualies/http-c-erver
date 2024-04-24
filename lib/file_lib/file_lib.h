#ifndef FILE_LIB_H
#define FILE_LIB_H

#include "../string_lib/string_lib.h"

/**
 * @brief Read a file
 * @waring The return value must be freed after use
 *
 * Returns NULL if the path is NULL, if the file does not exist or if the memory allocation fails.
 *
 * @param path The path to the file
 * @return The content of the file
 */
string *read_file(string *path);

#endif