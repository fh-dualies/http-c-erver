## To Do
### ``lib/http-lib/httplib.c``
- [ ] (line 6) : Let's split this file into smaller files based on the functions (e.g. stringlib.c,

- [ ] (line 10) : Let's sort the functions based on the lifecycle of the string (e.g. creation, manipulation,

- [ ] (line 171) : Let's move this function to a new file filelib.c

- [ ] (line 178) : is this safe?


### ``scripts/todo.sh``
- [ ] (line 2) "

- [ ] (line 23) )"


### ``src/http_parser/http_parser.c``
- [ ] (line 80) : Would it make sense to factor out the loop into a separate function? Just for readability purposes

- [ ] (line 97) : Ensure that the header starts at the beginning of the line

- [ ] (line 98) : remove strstr()

- [ ] (line 104) : remove strlen()

- [ ] (line 218) : fix Clang-Tidy warning


### ``src/http_router/http_router.c``
- [ ] (line 48) : is realpath() safe to use here?

- [ ] (line 76) : remove strstr()

- [ ] (line 82) : remove strstr()

- [ ] (line 88) : remove strstr()


### ``tests/http/tests.py``
- [ ] (line 102) : Enable these tests once the server correctly handles the Host header

- [ ] (line 113) : Add more tests for Host

- [ ] (line 140) : Add more tests for miscellaneous


### ``tests/unit/tests.c``
- [ ] (line 15) : check for missing test

######Generated using [todo.md](https://github.com/charlesthomas/todo.md)
