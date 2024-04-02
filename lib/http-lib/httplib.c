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

  char *new_string = realloc(dest->str, new_len + 1);

  if (new_string == NULL) {
    exit(4);
  }

  // copy src to dest
  memcpy(dest->str + dest->len, src, len);

  new_string[new_len] = '\0';

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

string *url_decode(string *str) {
  assert(str != NULL);
  assert(str->str != NULL);

  string *decoded = _new_string();

  for (int i = 0; i < str->len; i++) {
    char current = str->str[i];

    // plus signs should be decoded as spaces
    if (current == '+') {
      str_cat(decoded, " ", 1);
      continue;
    }

    if (current == '%' && i + 1 < str->len) {
      char new_char = '\0';
      char first = str->str[i + 1];
      char second = str->str[i + 2];

      if (first >= '0' && first <= '9') {
        new_char = first - '0';
      } else if (first >= 'a' && first <= 'f') {
        new_char = first - 'a' + 10;
      } else if (first >= 'A' && first <= 'F') {
        new_char = first - 'A' + 10;
      }

      new_char <<= 4;

      if (second >= '0' && second <= '9') {
        new_char |= second - '0';
      } else if (second >= 'a' && second <= 'f') {
        new_char |= second - 'a' + 10;
      } else if (second >= 'A' && second <= 'F') {
        new_char |= second - 'A' + 10;
      }

      str_cat(decoded, &new_char, 1);
      i += 2;
      continue;
    }

    // other characters should be copied
    str_cat(decoded, str->str + i, 1);
  }
  return decoded;
}

string *url_encode(string *str) {
  assert(str != NULL);
  assert(str->str != NULL);

  string *encoded = _new_string();
  const char *hex = "0123456789ABCDEF";

  for (int i = 0; i < str->len; i++) {
    char current = str->str[i];

    // normal characters should be copied
    if (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z' ||
        current >= '0' && current <= '9') {
      str_cat(encoded, str->str + i, 1);
      continue;
    }

    // spaces should be encoded as '+'
    if (current == ' ') {
      str_cat(encoded, "+", 1);
      continue;
    }

    // other characters should be encoded as '%XX'
    str_cat(encoded, "%", 1);
    str_cat(encoded, hex + (current >> 4), 1);
    str_cat(encoded, hex + (current & 0x0f), 1);
  }

  return encoded;
}