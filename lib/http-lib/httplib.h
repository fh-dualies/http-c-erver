#ifndef ECHO_SERVER_HTTPLIB_H
#define ECHO_SERVER_HTTPLIB_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct string {
    size_t len;
    size_t pos;
    char* str;
};
typedef struct string string;

string* str_cat(string* dest, const char* src, size_t len);
string* new_string(size_t );
void print_string(string* str);
string* cpy_str(const char* src, size_t len);
void free_str(string* str);
size_t get_length(string* str);
char* get_char_str(string* str);

#endif //ECHO_SERVER_HTTPLIB_H
