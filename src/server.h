#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "network_utils.h"
#include "utils.h"
#include "epoll_utils.h"
#include "constants.h"
#include "threads.h"


typedef struct {
  struct sockaddr_in address;
} initial_config_t ;

typedef struct {
  int socket;

  int waiting_clients_count;
  int waiting_clients[GROUP_SIZE];

  int clients_fds[MAX_CONNECTIONS];
  int clients_count;

  shared_data_t *threads_shared_data;
} server_config_t;

initial_config_t *parse_command_line_args(int argc, char **argv);

void run_server(initial_config_t *);

#endif
