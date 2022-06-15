
#include "server.h"


// setting up listening socket
int set_up_socket(initial_config_t *config) {
  int server_socket = make_socket(AF_INET, SOCK_STREAM);

  if (bind(server_socket, (struct sockaddr *)&config->address,
           sizeof(config->address)) < 0) {
    ERR("bind");
  }

  if (listen(server_socket, BACKLOG)) {
    ERR("listen");
  }

  return server_socket;
}


server_config_t *prepare_server(initial_config_t *config) {
  server_config_t *out = malloc(sizeof(server_config_t));


  out->socket = set_up_socket(config);

  out->clients_count = 0;

  return out;
}

void reject_connection(int fd, char *reason) {
  if (send(fd, reason, strlen(reason), 0) < 0)
    ERR("send");

    if (close(fd))
      ERR("close");
}

void handle_new_connection(server_config_t *config) {
  int client_fd = accept(config->socket, NULL, NULL);

  if (client_fd < 0)
    ERR("accept");

  if (config->clients_count == MAX_CONNECTIONS) {
    reject_connection(client_fd, "Server overloaded, try later.");

    return;
  }

  shared_data_t *data = config->threads_shared_data;

  pthread_mutex_lock(&data->waiting_mutex);

  data->waiting_clients[data->waiting_clients_count++] = client_fd;

  pthread_mutex_unlock(&data->waiting_mutex);

  printf("new connection handled\n");
}

void dispatch_waiting_clients(server_config_t *config) {
  printf("group formed\n");

  add_new_game(config->threads_shared_data);
}

void handle_events(struct epoll_event *events, int count, server_config_t* config) {
  for (int i = 0; i < count; i++) {
    if (events[i].data.fd == config->socket) {
      handle_new_connection(config);

      if (config->threads_shared_data->waiting_clients_count == GROUP_SIZE) {
        dispatch_waiting_clients(config);
      }
    }
  }
}

// function handles connections from both network and console
void handle_connections(server_config_t* config) {
  struct epoll_event events[MAX_CONNECTIONS];

  int epollfd = get_epoll();

  int count;

  epoll_add_read(epollfd, config->socket);

  for (;;) {
    if ((count = epoll_wait(epollfd, events, MAX_CONNECTIONS, -1)) == -1)
      ERR("epoll_wait");

    handle_events(events, count, config);
  }
}

// main function wchich perform server task
void server_work(server_config_t* config) {

  config->threads_shared_data = start_threads();

  handle_connections(config);
}

// prepares configuration and starts the server
void run_server(initial_config_t *config) {
  server_config_t *server_config = prepare_server(config);

  free(config);


  server_work(server_config);

  free(server_config);
}

initial_config_t *parse_command_line_args(int argc, char **argv) {
  initial_config_t *out = malloc(sizeof(initial_config_t));

  out->address = make_address(argv[2], argv[1]);

  return out;
}
