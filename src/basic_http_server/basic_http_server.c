#include "../../lib/http-lib/httplib.h"

string* basic_http_server(string* request) {
    printf("Received request: %s\n", get_char_str(request));

    return request;
}