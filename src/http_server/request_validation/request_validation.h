//
// Created by dennis on 23/04/24.
//

#ifndef PSE_2024_REQUEST_VALIDATION_H
#define PSE_2024_REQUEST_VALIDATION_H

#include <stdbool.h>
#include "../../http_models/http_models.h"

/**
 * Check if the request contains all necessary information
 * @param request
 * @return true if the request contains all necessary information
 */
bool request_empty(request_t *request);

/**
 * Check if the version is supported
 * @param version
 * @return true if the version is supported
 */
bool supported_version(string *version);

/**
 * Check if the method is supported
 * @param method
 * @return true if the method is supported
 */
bool supported_method(string *method);

#endif //PSE_2024_REQUEST_VALIDATION_H
