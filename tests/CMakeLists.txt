cmake_minimum_required(VERSION 3.26)
project(pse_2024_tests C)

set(CMAKE_C_STANDARD 11)

include_directories(../lib/unity/src)

add_library(unity STATIC ../lib/unity/src/unity.c)

add_executable(pse_2024_tests
        lib/http-lib/test_httplib.c)

target_link_libraries(pse_2024_tests unity)

set_target_properties(pse_2024_tests PROPERTIES SUFFIX ".out")