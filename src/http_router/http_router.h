//
// Created by dennis on 23/04/24.
//

#ifndef PSE_2024_HTTP_ROUTER_H
#define PSE_2024_HTTP_ROUTER_H

#include <stdbool.h>
#include "../http_models/http_models.h"

#define ROUTE_DEBUG "/debug"
#define HOST_DEFAULT_FOLDER "/default"

string *get_host_folder(request_t *request);

string *convert_to_absolute_path(string *resource, string *host_extension);

bool valid_path(string *path, string *host_extension);

string *route_request(request_t *request);

#endif //PSE_2024_HTTP_ROUTER_H
