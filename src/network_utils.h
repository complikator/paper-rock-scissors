#ifndef NETWORK_UTILS_h
#define NETWORK_UTILS_h

#include <netinet/in.h>

// creates new socket and checks for errors
int make_socket(int domain, int type);

// creates new ipv4 address
struct sockaddr_in make_address(char *port, char *addr);

#endif
