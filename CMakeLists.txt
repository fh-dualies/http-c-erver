cmake_minimum_required(VERSION 3.22)
project(pse_2024 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Enable Address Sanitizer for Debug builds
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

add_executable(tests
        lib/http-lib/httplib.c
        lib/testing/test-lib.c
        lib/testing/test-lib.h
        tests/httplib-test.c)

add_executable(server
        lib/http-lib/httplib.c
        src/http_server/http_server.c
        main.c)

set_target_properties(tests PROPERTIES SUFFIX ".out")
set_target_properties(server PROPERTIES SUFFIX ".out")