#include "file_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

string *read_file(string *path) {
  if (path == NULL) {
    return NULL;
  }

  struct stat s;

  if (stat(get_char_str(path), &s) != 0) {
    return NULL;
  }

  if (s.st_mode & S_IFDIR) {
    return NULL;
  }

  FILE *file = fopen(get_char_str(path), "r");

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
  fread(get_char_str(content), 1, length, file);
  content->len = length;

  fclose(file);

  return content;
}