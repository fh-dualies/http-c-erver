## To Do
### ``lib/file_lib/file_lib.c``
- [ ] (line 12) : is this safe?


### ``lib/string_lib/string_lib.c``
- [ ] (line 5) : Let's sort the functions based on the lifecycle of the string (e.g. creation, manipulation,


### ``scripts/todo.sh``
- [ ] (line 2) "

- [ ] (line 23) )"


### ``src/http_parser/http_parser.c``
- [ ] (line 4) : check for \0

- [ ] (line 63) : check for \0

- [ ] (line 82) : Would it make sense to factor out the loop into a separate function? Just for

- [ ] (line 100) : Ensure that the header starts at the beginning of the line

- [ ] (line 101) : remove strstr()

- [ ] (line 107) : remove strlen()

- [ ] (line 221) : fix Clang-Tidy warning


### ``src/http_router/http_router.c``
- [ ] (line 49) : is realpath() safe to use here?

- [ ] (line 77) : remove strstr()

- [ ] (line 83) : remove strstr()

- [ ] (line 89) : remove strstr()


### ``tests/http/tests.py``
- [ ] (line 102) : Enable these tests once the server correctly handles the Host header

- [ ] (line 113) : Add more tests for Host

- [ ] (line 122) : Check for \0 in request line & headers

- [ ] (line 142) : Add more tests for miscellaneous


### ``tests/unit/tests.c``
- [ ] (line 15) : check for missing test

######Generated using [todo.md](https://github.com/charlesthomas/todo.md)
