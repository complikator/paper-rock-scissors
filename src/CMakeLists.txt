cmake_minimum_required(VERSION 3.10)

# set the project name
project(tcp_server)

# add the executable
add_executable(tcp_server main.c utils.c server.c threads.c epoll_utils.c network_utils.c game.c)
