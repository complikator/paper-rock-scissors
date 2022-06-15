#ifndef UTILS_H
#define UTILS_H

#define ERR(source)                                                            \
  (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__),             \
   exit(EXIT_FAILURE))

void usage();

int find_index_of_element(int *array, int size, int element);


#endif
