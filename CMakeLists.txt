cmake_minimum_required(VERSION 3.16.3)
project(http-c-erver C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(tests
        lib/string_lib/string_lib.c
        lib/file_lib/file_lib.c
        src/http_parser/http_parser.c
        src/http_models/http_models.c
        src/http_server/http_server.c
        lib/testing/unit/test-lib.c
        tests/unit/http-lib/http-lib_test.c
        tests/unit/http_models/http_models_test.c
        tests/unit/http_parser/http_parser_test.c
        tests/unit/http_server/http_server_test.c
        tests/unit/tests.c
        src/http_server/request_validation/request_validation.c
        src/http_server/request_validation/request_validation.h
        src/http_router/http_router.c
        src/http_router/http_router.h
        tests/unit/http_server/request_validation/request_validation_test.c
        tests/unit/http_server/request_validation/request_validation_test.h
        tests/unit/http_router/http_router_test.c
        tests/unit/http_router/http_router_test.h)

add_executable(server
        lib/string_lib/string_lib.c
        lib/file_lib/file_lib.c
        src/http_models/http_models.c
        src/http_parser/http_parser.c
        src/http_server/http_server.c
        src/http_server/request_validation/request_validation.c
        src/http_server/request_validation/request_validation.h
        src/http_router/http_router.c
        src/http_router/http_router.h
        main.c)

add_executable(server_asan
        lib/string_lib/string_lib.c
        lib/file_lib/file_lib.c
        src/http_models/http_models.c
        src/http_parser/http_parser.c
        src/http_server/http_server.c
        src/http_server/request_validation/request_validation.c
        src/http_server/request_validation/request_validation.h
        src/http_router/http_router.c
        src/http_router/http_router.h
        main.c)

set_target_properties(tests PROPERTIES SUFFIX ".out")
set_target_properties(server PROPERTIES SUFFIX ".out")
set_target_properties(server_asan PROPERTIES SUFFIX ".out")

# AddressSanitizer
set_target_properties(server_asan PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set_target_properties(server_asan PROPERTIES LINK_FLAGS "-fsanitize=address")
