#include "httplib.h"
#include "../../src/http_server/http_server.h"
#include <assert.h>
#include <limits.h>
#include <sys/stat.h>

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

  dest->str = realloc(dest->str, new_len + 1);

  if (dest->str == NULL) {
    exit(4);
  }

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
    exit(4);
  }

  // copy src to dest
  memcpy(dest->str, src, len);

  dest->str[len] = '\0';

  dest->len = len;

  return dest;
}

string *_new_string() {
  string *str = calloc(1, sizeof(string));

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

  string *dest = calloc(1, sizeof(string));
  if (dest == NULL) {
    exit(2);
  }

  dest->str = calloc(1, len + 1);

  if (dest->str == NULL) {
    exit(3);
  }

  memcpy(dest->str, src, len);
  dest->str[len] = '\0';
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

string *read_file(char *path) {
  assert(path != NULL);

  struct stat s;
  if (stat(path, &s) != 0) {
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

  FILE *file = fopen(path, "r");

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
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
  }

  snprintf(temp_str, max_length + 1, "%zu", num);

  str = str_set(str, temp_str, max_length);
  free(temp_str);
  return str;
}

char *get_absolute_path(string *resource) {
  if (resource == NULL) {
    return NULL;
  }

  string *relative_path = _new_string();
  char *absolute_path = calloc(PATH_MAX, 1);

  if (absolute_path == NULL) {
    free_str(relative_path);
    return NULL;
  }

  str_cat(relative_path, DOCUMENT_ROOT, strlen(DOCUMENT_ROOT));
  str_cat(relative_path, resource->str, resource->len);

  char *real_path = realpath(relative_path->str, absolute_path);

  if (real_path == NULL) {
    free_str(relative_path);
    free(absolute_path);
    return NULL;
  }

  free_str(relative_path);

  return absolute_path;
}

const char *get_mime_type(char *path) {
  if (path == NULL) {
    return NULL;
  }

  const char *extension = strrchr(path, '.');

  if (extension == NULL) {
    return CONTENT_TYPE_TEXT;
  }

  if (strcmp(extension, ".html") == 0) {
    return CONTENT_TYPE_HTML;
  }

  if (strcmp(extension, ".css") == 0) {
    return CONTENT_TYPE_CSS;
  }

  if (strcmp(extension, ".js") == 0) {
    return CONTENT_TYPE_JS;
  }

  if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
    return CONTENT_TYPE_JPEG;
  }

  if (strcmp(extension, ".png") == 0) {
    return CONTENT_TYPE_PNG;
  }

  if (strcmp(extension, ".ico") == 0) {
    return CONTENT_TYPE_ICO;
  }

  return CONTENT_TYPE_TEXT;
}