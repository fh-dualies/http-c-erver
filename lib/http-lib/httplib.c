#include "httplib.h"
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>

void *exit_err(const char *function_name, const char *reason) {
  fprintf(stderr, "Error - %s(): %s\n", function_name, reason);
  exit(EXIT_FAILURE);
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

void str_to_lower(string *input) {
  if (input == NULL || input->str == NULL) {
    return;
  }

  for (size_t i = 0; i < input->len; i++) {
    input->str[i] = (char)tolower(input->str[i]);
  }
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

void print_string(string *str) {
  for (int i = 0; i < str->len; i++) {
    putchar(str->str[i]);
  }
}

string *cpy_str(const char *src, size_t len) {
  assert(src != NULL);

  string *dest = calloc(1, sizeof(string));

  if (dest == NULL) {
    return exit_err("cpy_str", "Memory allocation of dest failed.");
  }

  dest->str = calloc(1, len + 1);

  if (dest->str == NULL) {
    return exit_err("cpy_str", "Memory allocation of dest->str failed.");
  }

  memcpy(dest->str, src, len);
  dest->str[len] = '\0';
  dest->len = len;

  return dest;
}

void free_str(string *str) {
  if (str == NULL || str->str == NULL) {
    return;
  }

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

string *read_file(string *path) {
  if (path == NULL) {
    return NULL;
  }

  struct stat s;
  if (stat(path->str, &s) != 0) {
    return NULL;
  }

  if (s.st_mode & S_IFDIR) {
    return NULL;

    // We could add index.html to the path here to serve a default file
    /*string *new_path = cpy_str(path, strlen(path));
    str_cat(new_path, "/index.html", 11);
    string* file_content = read_file(new_path->str);
    free_str(new_path);
    return file_content;*/
  }

  FILE *file = fopen(path->str, "r");

  if (file == NULL) {
    return NULL;
  }

  // get file length
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  string *content = _new_string();
  free(content->str);
  content->str = calloc(length + 1, 1);

  if (content->str == NULL) {
    return NULL;
  }

  // read file content
  fread(content->str, 1, length, file);
  content->len = length;

  fclose(file);

  return content;
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