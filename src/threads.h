
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "utils.h"
#include "game.h"
#include "epoll_utils.h"

typedef struct {
  int id;
  int fds[GROUP_SIZE];
  pthread_mutex_t *waiting_mutex;
  pthread_cond_t *waiting_cond;

  int *waiting_clients;
  int *waiting_clients_count;
} thread_data_t;

typedef struct {
  pthread_t tids[MAX_CONNECTIONS];
  thread_data_t threads_data[MAX_CONNECTIONS];

  int clients_fds[GROUP_SIZE];

  int waiting_clients[GROUP_SIZE];
  int waiting_clients_count;
  pthread_mutex_t waiting_mutex;
  pthread_cond_t waiting_cond;
} shared_data_t;

shared_data_t *start_threads();

void add_new_game(shared_data_t* );
