#!/bin/bash

THIS_PATH="$(realpath "$0")"
THIS_DIR="$(dirname "$THIS_PATH")"

FILE_LIST="$(find "$THIS_DIR/../" -type f -name "*.c" -o -name "*.h")"

echo -e "Files found to format = \n\"\"\"\n$FILE_LIST\n\"\"\""

clang-format --verbose -i $FILE_LIST
