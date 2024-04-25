#include "string_lib.h"
#include <assert.h>
#include <ctype.h>

void *exit_err(const char *function_name, const char *reason) {
  fprintf(stderr, "Error - %s(): %s\n", function_name, reason);
  exit(EXIT_FAILURE);
}

string *_new_string() {
  string *str = calloc(1, sizeof(string));

  if (str == NULL) {
    return exit_err("_new_string", "Memory allocation of str failed.");
  }

  str->str = calloc(1, 1);

  if (str->str == NULL) {
    return exit_err("_new_string", "Memory allocation of str->str failed.");
  }

  str->str[0] = '\0';
  str->len = 0;

  return str;
}

string *str_set(string *dest, const char *src, size_t len) {
  // check if dest is NULL
  if (dest == NULL) {
    return NULL;
  }

  // check if src is NULL or len is 0
  if (src == NULL || len <= 0) {
    return dest;
  }

  free(dest->str);

  dest->str = calloc(len + 1, 1);

  if (dest->str == NULL) {
    return exit_err("str_set", "Memory allocation of dest->str failed.");
  }

  // copy src to dest
  memcpy(dest->str, src, len);

  dest->str[len] = '\0';

  dest->len = len;

  return dest;
}

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

  void *temp = realloc(dest->str, new_len + 1);

  if (temp == NULL) {
    return exit_err("str_cat", "Memory allocation if temp failed.");
  }

  dest->str = temp;

  // copy src to dest
  memcpy(dest->str + dest->len, src, len);

  dest->str[new_len] = '\0';
  dest->len = new_len;

  return dest;
}

string *str_cpy(const char *src, size_t len) {
  assert(src != NULL);

  string *dest = calloc(1, sizeof(string));

  if (dest == NULL) {
    return exit_err("str_cpy", "Memory allocation of dest failed.");
  }

  dest->str = calloc(1, len + 1);

  if (dest->str == NULL) {
    return exit_err("str_cpy", "Memory allocation of dest->str failed.");
  }

  memcpy(dest->str, src, len);
  dest->str[len] = '\0';
  dest->len = len;

  return dest;
}

int str_cmp(string *str1, const char *str2) {
  if (str1 == NULL || str2 == NULL) {
    return -1;
  }

  if (str1->len != strlen(str2)) {
    return -1;
  }

  return memcmp(str1->str, str2, str1->len);
}

void str_to_lower(string *input) {
  if (input == NULL || input->str == NULL) {
    return;
  }

  for (size_t i = 0; i < input->len; i++) {
    input->str[i] = (char)tolower(input->str[i]);
  }
}

string *int_to_string(int num) {
  string *str = _new_string();
  int max_length = snprintf(NULL, 0, "%d", num);

  char *temp_str = (char *)calloc(max_length + 1, sizeof(char));

  if (temp_str == NULL) {
    return exit_err("int_to_string", "Memory allocation of temp_str failed.");
  }

  snprintf(temp_str, max_length + 1, "%d", num);

  str = str_set(str, temp_str, max_length);
  free(temp_str);
  return str;
}

string *size_t_to_string(size_t num) {
  string *str = _new_string();
  int max_length = snprintf(NULL, 0, "%zu", num);

  char *temp_str = (char *)calloc(max_length + 1, sizeof(char));

  if (temp_str == NULL) {
    return exit_err("size_t_to_string", "Memory allocation of temp_str failed.");
  }

  snprintf(temp_str, max_length + 1, "%zu", num);

  str = str_set(str, temp_str, max_length);
  free(temp_str);
  return str;
}

void print_str(string *str) {
  for (int i = 0; i < str->len; i++) {
    putchar(str->str[i]);
  }
}

size_t get_length(string *str) {
  if (str == NULL) {
    exit_err("get_length", "str is NULL.");
    return -1;
  }

  return str->len;
}

char *get_char_str(string *str) {
  if (str == NULL) {
    return exit_err("get_char_str", "str is NULL.");
  }

  if (str->str[str->len] != '\0') {
    return exit_err("get_char_str", "str does not have null terminator.");
  }

  return str->str;
}

void free_str(string *str) {
  if (str == NULL || str->str == NULL) {
    return;
  }

  free(str->str);
  free(str);
}