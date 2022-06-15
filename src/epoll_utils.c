
#include "epoll_utils.h"

int get_epoll() {
  int fd;

  if ((fd = epoll_create(1)) < 0)
    ERR("epoll_create");

  return fd;
}

void epoll_add(int epollfd, int file_descriptor, int events_flags) {
  struct epoll_event ev;

  ev.events = events_flags;
  ev.data.fd = file_descriptor;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, file_descriptor, &ev) == -1)
    ERR("epoll_ctl");
}

void epoll_add_read(int epollfd, int file_descriptor) {
  epoll_add(epollfd, file_descriptor, EPOLLIN);
}

void epoll_add_write(int epollfd, int file_descriptor) {
  epoll_add(epollfd, file_descriptor, EPOLLOUT);
}

void epoll_add_read_write(int epollfd, int file_descriptor) {
  epoll_add(epollfd, file_descriptor, EPOLLIN | EPOLLOUT);
}
