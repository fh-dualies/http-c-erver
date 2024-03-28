cmake_minimum_required(VERSION 3.26)
project(pse_2024 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(http_lib_test lib/http-lib/httplib.c lib/http-lib/httplib-test.c)
add_executable(echo_server lib/http-lib/httplib.c echo_server.c)

set_target_properties(http_lib_test PROPERTIES SUFFIX ".out")
set_target_properties(echo_server PROPERTIES SUFFIX ".out")