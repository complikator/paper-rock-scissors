
#include "threads.h"

shared_data_t *prepare_shared() {
  shared_data_t *out = malloc(sizeof(shared_data_t));

  if (pthread_mutex_init(&out->groups_availability_mutex, NULL))
    ERR("pthread_mutex_init");

  for (int i = 0; i < MAX_CONNECTIONS / GROUP_SIZE; i++) {
    if (pthread_cond_init(&out->cond_variables[i], NULL))
      ERR("pthread_cond_init");

    if (pthread_mutex_init(&out->scores_mutexes[i], NULL))
      ERR("pthread_mutex_init");

    out->is_group_available[i] = 1;
  }

  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    out->threads_data[i].id = i % GROUP_SIZE;

    out->threads_data[i].scores_mutex = &out->scores_mutexes[i / GROUP_SIZE];

    out->threads_data[i].is_group_available = &out->is_group_available[i / GROUP_SIZE];
  }

  out->current_group = 0;

  return out;
}

void *thread_func(void *arg) {
  thread_data_t *targ = (thread_data_t*)arg;

  printf("[%d] started\n", targ->id);

  return NULL;
}

void create_threads(shared_data_t *data) {

  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    if (pthread_create(&data->tids[i], NULL, thread_func, &data->threads_data[i]))
      ERR("pthread_create");
  }
}

shared_data_t *start_threads() {
  shared_data_t *data = prepare_shared();

  create_threads(data);

  return data;
}


