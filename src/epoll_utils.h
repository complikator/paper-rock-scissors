
#include <sys/epoll.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


// returns new file descriptor indicating epoll instance
int get_epoll();


void epoll_add_read(int epollfd, int file_descriptor);

void epoll_add_write(int epollfd, int file_descriptor);

void epoll_add_read_write(int epollfd, int file_descriptor);
