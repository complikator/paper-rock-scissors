
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "network_utils.h"
#include "utils.h"

int make_socket(int domain, int type) {
  int sock;

  if ((sock = socket(domain, type, 0)) < 0) {
    ERR("socket");
  }

  return sock;
}

struct sockaddr_in make_address(char *port, char *address) {
  int ret;
  struct sockaddr_in addr;
  struct addrinfo *result;
  struct addrinfo hints = {};

  hints.ai_family = AF_INET;

  if ((ret = getaddrinfo(address, port, &hints, &result))) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  addr = *(struct sockaddr_in *)(result->ai_addr);
  freeaddrinfo(result);
  return addr;
}
