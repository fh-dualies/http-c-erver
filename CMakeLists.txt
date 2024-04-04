cmake_minimum_required(VERSION 3.26)
project(pse_2024 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(tests
        lib/http-lib/httplib.c
        lib/testing/test-lib.c
        lib/testing/test-lib.h
        tests/httplib-test.c)

add_executable(server
        lib/http-lib/httplib.c
        main.c
        src/basic_http_server/basic_http_server.c)

set_target_properties(tests PROPERTIES SUFFIX ".out")
set_target_properties(server PROPERTIES SUFFIX ".out")