cmake_minimum_required(VERSION 3.26)
project(pse_2024 C)

set(CMAKE_C_STANDARD 11)

add_executable(pse_2024 lib/http-lib/httplib.c main.c)

set_target_properties(pse_2024 PROPERTIES SUFFIX ".out")
