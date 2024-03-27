cmake_minimum_required(VERSION 3.26)
project(pse_2024_tests C)

set(CMAKE_C_STANDARD 11)

add_executable(pse_2024_tests
        lib/http-lib/test_httplib.c)

set_target_properties(pse_2024_tests PROPERTIES SUFFIX ".out")