#include "httplib.h"
#include <assert.h>

string *str_cat(string *dest, const char *src, size_t len) {
  // check if dest is NULL
  if (dest == NULL) {
    return NULL;
  }

  // length after concatenation
  size_t new_len = dest->pos + len;

  // resize dest if necessary
  if (new_len >= dest->len) {
    char *new_string = realloc(dest->str, new_len * 2);

    if (new_string == NULL) {
      exit(4);
    }

    dest->len = new_len * 2;
    dest->str = new_string;
  }

  // copy src to dest
  memcpy(dest->str + dest->pos, src, len);
  dest->pos = new_len;

  return dest;
}

string *new_string(size_t len) {
  string *str = calloc(sizeof(string), 1);
  if (str == NULL) {
    exit(2);
  }

  str->str = calloc(len, 2);
  if (str->str == NULL) {
    exit(3);
  }

  str->len = len * 2;
  str->pos = 0;

  return str;
}

void print_string(string *str) {
  for (int i = 0; i <= str->pos; i++) {
    putchar(str->str[i]);
  }
}

string *cpy_str(const char *src, size_t len) {
  string *dest = new_string(len);
  memcpy(dest->str, src, len);
  dest->pos = len - 1;

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
  return str->pos + 1;
}

char *get_char_str(string *str) {
  assert(str != NULL);
  return str->str;
}
