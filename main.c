#include "main.h"
#include "lib/string_lib/string_lib.h"
#include "src/http_server/http_server.h"
#include <errno.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static bool run = true;

/**
 * @brief Handle the socket signal
 *
 * Exists with error if the signal is not SIGINT
 *
 * @param signum
 */
static void handle_signal(int signum) {
  if (signum != SIGINT) {
    exit_err("handle_signal", "unexpected signal");
  }

  // exit the server after the last client has been processed.
  run = false;
}

/**
 * @brief Registers the signal handler for SIGINT
 *
 * Exists with error if the signal handler could not be registered
 */
static void register_signal() {
  struct sigaction action;

  // configure the signal handler
  memset(&action, 0, sizeof(action));
  action.sa_handler = handle_signal;

  // register the signal handler
  if (sigaction(SIGINT, &action, NULL) < 0) {
    exit_err("register_signal", "registering signal handler");
  }
}

/**
 * @brief Set up the socket
 *
 * Creates a socket and binds it to the server configuration.
 * Exits with error if the socket could not be created or bound.
 *
 * @return the socket file descriptor
 */
static int setup_socket() {
  int opt = 1;
  int sock_fd = 0;
  struct sockaddr_in serv_addr;

  // set the server configuration
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(SERVER_PORT);

  // create the socket
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (sock_fd < 0) {
    exit_err("setup_socket", "opening socket");
  }

  // allow the socket to be reused
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int)) < 0) {
    exit_err("setup_socket", "on setsockopt");
  }

  // allow the socket to accept connections
  if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    exit_err("setup_socket", "on binding");
  }

  // check for maximum connections
  if (listen(sock_fd, SERVER_MAX_CONNECTIONS) < 0) {
    exit_err("setup_socket", "listen");
  }

  return sock_fd;
}

/**
 * @brief Process the incoming request
 *
 * Processes the incoming request and returns the response.
 *
 * @param request the incoming request
 * @return the response
 */
string *process(string *request) {
  string *response = http_server(request);
  free_str(request);

  return response;
}

/**
 * @brief Main loop for the server (stdin)
 *
 * The main loop for the server. Accepts incoming connections and processes them.
 */
static void main_loop_stdin() {
  void *const buffer = malloc(SERVER_BUFFER_SIZE);

  if (buffer == NULL) {
    exit_err("main_loop_stdin", "at malloc.");
  }

  // read incoming data from buffer
  memset(buffer, 0, SERVER_BUFFER_SIZE);
  size_t length = read(STDIN_FILENO, buffer, SERVER_BUFFER_SIZE - 1);

  if (length < 0 && errno != EINTR) {
    exit_err("main_loop_stdin", "reading from socket");
  }

  string *request = str_cpy(buffer, length);
  string *response = process(request);

  size_t response_len = get_length(response);
  char *response_char = get_char_str(response);

  // write outgoing data to buffer
  if (write(STDOUT_FILENO, response_char, response_len) < 0) {
    exit_err("main_loop_stdin", "writing to STDOUT");
  }

  free_str(response);
  free(buffer);
}

/**
 * @brief Main loop for the server
 *
 * The main loop for the server. Accepts incoming connections and processes them.
 */
static void main_loop() {
  const int sock_fd = setup_socket();
  int new_sock_fd;
  ssize_t length;

  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  void *const buffer = malloc(SERVER_BUFFER_SIZE);

  if (buffer == NULL) {
    exit_err("main_loop", "at malloc.");
  }

  while (run) {
    // block until a client connects
    new_sock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);

    if (new_sock_fd < 0) {
      if (errno == EINTR) {
        break;
      }

      exit_err("main_loop", "on accept");
    }

    // read the incoming data from the socket
    memset(buffer, 0, SERVER_BUFFER_SIZE);
    length = read(new_sock_fd, buffer, SERVER_BUFFER_SIZE - 1);

    if (length < 0) {
      if (errno == EINTR) {
        break;
      }

      exit_err("main_loop", "reading from socket");
    }

    string *request = str_cpy(buffer, length);
    string *response = process(request);

    size_t response_len = get_length(response);
    char *response_char = get_char_str(response);

    // write the outgoing data to the socket
    length = write(new_sock_fd, response_char, response_len);

    if (length < 0) {
      exit_err("main_loop", "writing to socket");
    }

    free_str(response);

    // close the connection
    if (close(new_sock_fd) < 0) {
      exit_err("main_loop", "on close");
    }
  }

  free(buffer);

  if (close(sock_fd) < 0) {
    exit_err("main_loop", "on close");
  }
}

/**
 * @brief http-c-server
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @return the exit status
 */
int main(int argc, char *argv[]) {
  register_signal();

  if (argc == 2 && strcmp("stdin", argv[1]) == 0) {
    main_loop_stdin();
  } else {
    main_loop();
  }

  return 0;
}
