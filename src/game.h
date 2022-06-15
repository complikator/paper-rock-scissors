
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "utils.h"

typedef struct {
  int scores[GROUP_SIZE];
  char *current_signs[GROUP_SIZE];
  int players_ready;

} game_t;

game_t *init_game();

// returns positive value when round is finished
// or negative to indicate which id has invalid sign (- player_id)
int make_turn(int player_id, char *sign, game_t *game);

int is_game_over(game_t *game);

// give summary about current state of a game
void get_summary_per_player(game_t *game, char *summary_array, int size);
