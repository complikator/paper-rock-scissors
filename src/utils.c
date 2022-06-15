
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "server.h"

void usage() {
  printf("first argument is server address\n");
  printf("second argument is server port\n");

  exit(EXIT_FAILURE);
}

int find_index_of_element(int *array, int size, int element) {
  for (int i = 0; i < size; i++) {
    if (array[i] == element)
      return i;
  }

  return -1;
}

