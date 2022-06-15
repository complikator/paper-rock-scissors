
#include "threads.h"

shared_data_t *prepare_shared() {
  shared_data_t *out = malloc(sizeof(shared_data_t));

  if (pthread_mutex_init(&out->waiting_mutex, NULL))
    ERR("pthread_mutex_init");

  if (pthread_cond_init(&out->waiting_cond, NULL))
    ERR("pthread_cond_init");

  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    out->threads_data[i].id = i;

    out->threads_data[i].waiting_mutex = &out->waiting_mutex;

    out->threads_data[i].waiting_cond = &out->waiting_cond;

    out->threads_data[i].waiting_clients = out->waiting_clients;
    out->threads_data[i].waiting_clients_count = &out->waiting_clients_count;
  }

  out->waiting_clients_count = 0;

  return out;
}

void play_game(game_t *game_state, thread_data_t* t_data) {
  int epollfd = get_epoll();

  for (int i = 0; i < GROUP_SIZE; i++) {
    epoll_add_read(epollfd, t_data->fds[i]);
  }

  int count;

  struct epoll_event events[GROUP_SIZE];
  char buf[MAX_MSG + 1];
  char summary_array[GROUP_SIZE];
  char *ack_message = "Message received\n";

  while (1) {
    if ((count = epoll_wait(epollfd, events, GROUP_SIZE, -1)) == -1)
      ERR("epoll_wait");

    int index;
    for (int i = 0; i < count; i++) {
      if ((index = find_index_of_element(t_data->fds, GROUP_SIZE, events[i].data.fd)) == -1)
        continue;

      if (read(events[i].data.fd, buf, MAX_MSG) < 0)
        ERR("read");

      buf[MAX_MSG] = 0;

      if (make_turn(index, buf, game_state) > 0) {
        get_summary_per_player(game_state, summary_array, GROUP_SIZE);
      }

      if (write(events[i].data.fd, ack_message, strlen(ack_message)) < 0)
        ERR("send");
    }

  }
}

void start_game(thread_data_t *t_data) {
  printf("Game started\n");

  game_t *game_state = init_game();

  play_game(game_state, t_data);
}

void *thread_func(void *arg) {
  thread_data_t *targ = (thread_data_t*)arg;

  printf("[%d] started\n", targ->id);

  pthread_mutex_lock(targ->waiting_mutex);

  while (*targ->waiting_clients_count != GROUP_SIZE) {
    pthread_cond_wait(targ->waiting_cond, targ->waiting_mutex);
  }

  for (int i = 0; i < GROUP_SIZE; i++) {
    targ->fds[i] = targ->waiting_clients[i];
  }

  *targ->waiting_clients = 0;

  pthread_mutex_unlock(targ->waiting_mutex);

  start_game(targ);

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

void add_new_game(shared_data_t *data) {

  pthread_mutex_lock(&data->waiting_mutex);

  pthread_cond_signal(&data->waiting_cond);

  pthread_mutex_unlock(&data->waiting_mutex);
}
