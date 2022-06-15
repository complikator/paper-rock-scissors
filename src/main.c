#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "utils.h"

int main(int argc, char **argv) {

  if (argc < 3) {
    usage();
  }

  initial_config_t *server_config = parse_command_line_args(argc, argv);

  run_server(server_config);

  return EXIT_SUCCESS;
}
