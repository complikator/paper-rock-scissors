
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "utils.h"

typedef struct {
  int id;
  int fds[GROUP_SIZE];
  int scores[GROUP_SIZE];
  pthread_mutex_t *scores_mutex;

  int *is_group_available;
} thread_data_t;

typedef struct {
  pthread_t tids[MAX_CONNECTIONS];
  thread_data_t threads_data[MAX_CONNECTIONS];
  pthread_mutex_t scores_mutexes[MAX_CONNECTIONS / GROUP_SIZE];

  pthread_mutex_t groups_availability_mutex;
  pthread_cond_t cond_variables[MAX_CONNECTIONS / GROUP_SIZE];

  int is_group_available[MAX_CONNECTIONS / GROUP_SIZE];
  int current_group;

  int clients_fds[GROUP_SIZE];
} shared_data_t;

shared_data_t *start_threads();

void add_new(shared_data_t* );
