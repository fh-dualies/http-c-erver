#include "http_models.h"
#include "../../main.h"
#include "../http_server/http_server.h"

request_t *new_request() {
  request_t *request = calloc(1, sizeof(struct request_t));

  if (request == NULL) {
    return NULL;
  }

  request->method = _new_string();
  request->resource = _new_string();
  request->version = _new_string();
  request->host = _new_string();
  request->user_agent = _new_string();
  request->accept = _new_string();
  request->connection = _new_string();

  if (request->method == NULL || request->resource == NULL || request->version == NULL ||
      request->host == NULL || request->user_agent == NULL || request->accept == NULL ||
      request->connection == NULL) {
    free(request);
    return NULL;
  }

  return request;
}

response_t *new_response() {
  response_t *response = calloc(1, sizeof(struct response_t));

  if (response == NULL) {
    return NULL;
  }

  response->version = _new_string();
  response->status_code = _new_string();
  response->status_message = _new_string();
  response->content_type = _new_string();
  response->content_length = _new_string();
  response->server = _new_string();
  response->body = _new_string();

  if (response->version == NULL || response->status_code == NULL ||
      response->content_type == NULL || response->content_length == NULL ||
      response->body == NULL || response->server == NULL) {
    free(response);
    return NULL;
  }

  return response;
}

void free_request(request_t **request) {
  if (*request == NULL) {
    return;
  }

  free_str((*request)->resource);
  free_str((*request)->version);
  free_str((*request)->method);
  free_str((*request)->host);
  free_str((*request)->user_agent);
  free_str((*request)->accept);
  free_str((*request)->connection);
  free(*request);
  *request = NULL;
}

void free_response(response_t **response) {
  if (*response == NULL) {
    return;
  }

  free_str((*response)->version);
  free_str((*response)->status_code);
  free_str((*response)->status_message);
  free_str((*response)->content_type);
  free_str((*response)->content_length);
  free_str((*response)->server);
  free_str((*response)->body);
  free(*response);
  *response = NULL;
}

void update_response_content_length(response_t *response) {
  if (response == NULL) {
    return;
  }

  string *content_length = size_t_to_string(get_length(response->body));
  response->content_length =
      str_set(response->content_length, get_char_str(content_length), get_length(content_length));
  free_str(content_length);
}

void generate_response_status(response_t *response, int status_code, string *content_type) {
  if (response == NULL) {
    return;
  }

  string *status_code_str = int_to_string(status_code);
  const char *status_message = get_http_status_message(status_code);

  response->version =
      str_set(response->version, HTTP_DEFAULT_VERSION, strlen(HTTP_DEFAULT_VERSION));
  response->status_code =
      str_set(response->status_code, get_char_str(status_code_str), get_length(status_code_str));
  response->status_message =
      str_set(response->status_message, status_message, strlen(status_message));
  response->content_type =
      str_set(response->content_type, get_char_str(content_type), get_length(content_type));
  response->server = str_set(response->server, SERVER_SIGNATURE, strlen(SERVER_SIGNATURE));

  free_str(status_code_str);
}

void add_response_string_header(string *raw_string, const char *header, string *value) {
  if (raw_string == NULL || header == NULL || value == NULL) {
    return;
  }

  str_cat(raw_string, header, strlen(header));
  str_cat(raw_string, get_char_str(value), get_length(value));
  str_cat(raw_string, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));
}

void generate_response_status_line(string *raw_string, string *version, string *status_code,
                                   string *status_message) {
  str_cat(raw_string, get_char_str(version), get_length(version));
  str_cat(raw_string, " ", 1);
  str_cat(raw_string, get_char_str(status_code), get_length(status_code));

  str_cat(raw_string, " ", 1);
  str_cat(raw_string, get_char_str(status_message), get_length(status_message));

  str_cat(raw_string, HTTP_LINE_BREAK, strlen(HTTP_LINE_BREAK));
}