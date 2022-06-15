
#include "game.h"

game_t *init_game() {
  game_t *out = malloc(sizeof(game_t));

  for (int i = 0; i < GROUP_SIZE; i++) {
    out->scores[i] = 0;
    out->current_signs[i] = malloc(sizeof(char) * MAX_MSG);
  }

  return out;
}

void get_summary_per_player(game_t *game, char *summary_array, int size) {}

// returns 0 if every sign is ok, or first invalid index
int validate_signs(char **signs, int size) {
  for (int i = 0; i < size; i++) {
    if (strcmp(signs[i], PAPER) != 0 &&
        strcmp(signs[i], ROCK) != 0 &&
        strcmp(signs[i], SCISSORS) != 0){
      return i;
    }
  }

  return 0;
}

// returns winner id or -1 when no winner exist
int get_winner(char **signs, int size) {
  int signs_counter[3] = {0};


  for (int i = 0; i < size; i++) {
    if (strcmp(signs[i], PAPER) == 0) {
      signs_counter[0]++;
    }
    if (strcmp(signs[i], ROCK) == 0) {
      signs_counter[1]++;
    }
    if (strcmp(signs[i], SCISSORS) == 0) {
      signs_counter[2]++;
    }
  }

  if (signs_counter[0] * signs_counter[1] * signs_counter[2] > 0) {
    return -1;
  }

  if (signs_counter[0] == GROUP_SIZE ||
      signs_counter[1] == GROUP_SIZE ||
      signs_counter[2] == GROUP_SIZE){
    return -1;
  }

  for (int i = 0; i < 3; i++) {
    if (signs_counter[i] == 1)
      return i;
  }

  return -1;
}

int make_turn(int player_id, char *sign, game_t *game) {
  if (strcpy(game->current_signs[player_id], sign) == NULL)
    ERR("strcpy");

  game->players_ready++;

  if (game->players_ready == GROUP_SIZE) {
    int state;
    if ((state = validate_signs(game->current_signs, GROUP_SIZE))) {
      return -state;
    }

    state = get_winner(game->current_signs, GROUP_SIZE);

    if (state < 0) {
      game->scores[state]++;

      return state;
    }
  }

  return 0;
}

int is_game_over(game_t *game) {
  return 0;
}
