#ifndef PSE_2024_HTTP_ROUTER_H
#define PSE_2024_HTTP_ROUTER_H

#include "../http_models/http_models.h"
#include <stdbool.h>

#define ROUTE_DEBUG "/debug"
#define ROUTE_DEFAULT_HOST "/default"

/**
 * @brief Returns the folder for the specified host
 *
 * The folder is determined by the host field in the request.
 * If the host is NULL or does not match any of the predefined hosts,
 * the default folder is returned.
 *
 * Routes: (HOST: route)
 * - EXTERN: /extern
 * - INTERN: /intern
 * - DEFAULT: /default
 *
 * @param request the request object
 * @return string* of the subdirectory after document root
 */
string *get_host_directory(request_t *request);

/**
 * @brief Converts a relative path to an absolute path
 *
 * The absolute path is determined by the document root, host extension and resource.
 * The document root is the root directory of the server.
 * Returns NULL if the resource is NULL or the absolute path could not be resolved.
 *
 * @param resource the resource requested by the client
 * @param host_extension the subdirectory after the document root
 * @return string* of the absolute path
 */
string *convert_to_absolute_path(string *resource, string *host_extension);

/**
 * @brief Checks if the path is valid
 *
 * A path is valid if it is not NULL and the host extension is not NULL (Both must be longer than
 * 0).
 *
 * @param path the path to check
 * @param host_extension the host extension to check
 * @return true if the path is valid, false otherwise
 */
bool valid_path(string *path, string *host_extension);

/**
 * @brief Routes the request to the correct path
 * @warning will return a fully qualified HTTP response (either if the request is valid or not)
 * @warning will free the request object
 *
 * The path is determined by the host and the resource.
 * If the host is NULL or does not match any of the predefined hosts,
 * the default folder is returned.
 *
 * @param request the request object
 * @return string* of the absolute path
 */
string *route_request(request_t *request);

#endif
