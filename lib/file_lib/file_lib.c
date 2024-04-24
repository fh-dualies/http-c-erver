#include "file_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

string *read_file(string *path) {
  if (path == NULL) {
    return NULL;
  }

  struct stat s;
  // TODO: is this safe?
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