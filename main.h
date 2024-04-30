/**
 * @note General Server Configuration.
 * This section contains general server configuration options.
 */
#ifndef PSE_H
#define PSE_H

/**
 * The server signature.
 * This will be sent as the Server header in the response.
 * Feel free to change this to your own server signature.
 */
#define SERVER_SIGNATURE "LLDM/0.1 HTTP Server"

/**
 * The document root of the server.
 * @warning This path is relative to the project root - depending on the build
 * system, the path may need to be adjusted. For example, running the server.out in
 * the cli requires no change, but running the server via jetbrains IDE's
 * requires the path to be set to "../src/htdocs"
 */
#define DOCUMENT_ROOT "src/htdocs"

/**
 * Maximum size of a request.
 * By default, this is set to 8192 bytes (8KB). (reference:
 * https://www.rfc-editor.org/rfc/rfc9110.html#section-4.1-5)
 */
#define HTTP_MAX_REQUEST_SIZE 8192

/**
 * Default HTTP version used by the server for responses.
 */
#define HTTP_DEFAULT_VERSION "HTTP/1.1"

/**
 * Route definitions.
 */
#define ROUTE_DEBUG "/debug"
#define ROUTE_DEFAULT_HOST "/default"
#define ROUTE_EXTERN_HOST "/extern"
#define ROUTE_INTERN_HOST "/intern"

/**
 * Host definitions.
 */
#define HOST_INTERN "intern"
#define HOST_EXTERN "extern"

#endif