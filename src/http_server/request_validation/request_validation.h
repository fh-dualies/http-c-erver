#ifndef REQUEST_VALIDATION_H
#define REQUEST_VALIDATION_H

#include "../../http_models/http_models.h"
#include <stdbool.h>

/**
 * @brief Check if the request contains all necessary information
 *
 * The request must contain a method, a resource and a version which cannot be null or empty.
 *
 * @param request
 * @return true if the request contains all necessary information
 */
bool request_empty(request_t *request);

/**
 * @breif Check if the version is supported
 *
 * The version must be HTTP/1.0 or HTTP/1.1 (for the current state of implementation)
 *
 * @param version
 * @return true if the version is supported
 */
bool supported_version(string *version);

/**
 * @brief Check if the method is supported
 *
 * The method must be GET (for the current state of implementation)
 *
 * @param method
 * @return true if the method is supported
 */
bool supported_method(string *method);

#endif
