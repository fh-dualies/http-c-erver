#include "httplib.h"
#include <assert.h>

string *str_cat(string *dest, const char *src, size_t len) {
  // check if dest is NULL
  if (dest == NULL) {
    return NULL;
  }

  // check if src is NULL or len is 0
  if (src == NULL || len <= 0) {
    return dest;
  }

  // length after concatenation
  size_t new_len = dest->len + len;

  char *new_string = realloc(dest->str, new_len);

  if (new_string == NULL) {
    exit(4);
  }

  // copy src to dest
  memcpy(dest->str + dest->len, src, len);

  dest->len = new_len;
  dest->str = new_string;

  return dest;
}

string *_new_string() {
  string *str = calloc(sizeof(string), 1);
  if (str == NULL) {
    exit(2);
  }

  str->str = calloc(1, 1);
  if (str->str == NULL) {
    exit(3);
  }

  str->str[0] = '\0';

  str->len = 0;

  return str;
}

void print_string(string *str) {
  for (int i = 0; i < str->len; i++) {
    putchar(str->str[i]);
  }
}

string *cpy_str(const char *src, size_t len) {

  assert(src != NULL);

  string *dest = calloc(sizeof(string), 1);
  if (dest == NULL) {
    exit(2);
  }

  dest->str = calloc(1, len);
  if (dest->str == NULL) {
    exit(3);
  }
  memcpy(dest->str, src, len);
  dest->len = len;

  return dest;
}

void free_str(string *str) {
  assert(str != NULL);
  assert(str->str != NULL);
  free(str->str);
  free(str);
}

size_t get_length(string *str) {
  assert(str != NULL);
  return str->len;
}

char *get_char_str(string *str) {
  assert(str != NULL);
  return str->str;
}
