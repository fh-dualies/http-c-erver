cmake_minimum_required(VERSION 3.16.3)
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
        src/http_server/http_server.c
        main.c)

add_executable(server_asan
        lib/http-lib/httplib.c
        src/http_server/http_server.c
        main.c)

set_target_properties(tests PROPERTIES SUFFIX ".out")
set_target_properties(server PROPERTIES SUFFIX ".out")
set_target_properties(server_asan PROPERTIES SUFFIX ".out")

# AddressSanitizer
set_target_properties(server_asan PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set_target_properties(server_asan PROPERTIES LINK_FLAGS "-fsanitize=address")
