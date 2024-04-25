## To Do
### ``scripts/todo.sh``
- [ ] (line 2) "

- [ ] (line 23) )"


### ``src/http_parser/http_parser.c``
- [ ] (line 4) : check for \0

- [ ] (line 63) : check for \0

- [ ] (line 98) : Ensure that the header starts at the beginning of the line

- [ ] (line 99) : remove strstr()

- [ ] (line 105) : remove strlen()

- [ ] (line 219) : fix Clang-Tidy warning


### ``src/http_router/http_router.c``
- [ ] (line 76) : remove strstr()

- [ ] (line 82) : remove strstr()

- [ ] (line 88) : remove strstr()


### ``tests/http/tests.py``
- [ ] (line 102) : Enable these tests once the server correctly handles the Host header

- [ ] (line 113) : Add more tests for Host

- [ ] (line 122) : Check for \0 in request line & headers

- [ ] (line 142) : Add more tests for miscellaneous


### ``tests/unit/tests.c``
- [ ] (line 15) : check for missing test

######Generated using [todo.md](https://github.com/charlesthomas/todo.md)
