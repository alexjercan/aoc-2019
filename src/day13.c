#include "../include/day13.h"
#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_BLOCK 2
#define TILE_PADDLE 3
#define TILE_BALL 4

static void parse_input(char *input, struct program *p) {
  char *line = strtok(input, ",");
  size_t i = 0;

  while (line != NULL) {
    value_t data = atol(line);

    vector_set(p->memory, i, &data);
    line = strtok(NULL, ",");
    i++;
  }
}

static int part1(struct program *p) {
  while (program_step(p) != CODE_HALT)
    ;

  int count = 0;
  for (size_t i = 0; i < array_size(p->output); i += 3) {
    if (*(value_t *)array_get_ref(p->output, i + 2) == TILE_BLOCK)
      count++;
  }

  return count;
}

static int part2(struct program *p) {
  value_t quarters = 2;
  vector_set(p->memory, 0, &quarters);

  int code = CODE_OK;
  int score = 0;
  value_t joystick = 0;
  value_t paddle_x = 0;
  value_t ball_x = 0;
  while (code != CODE_HALT) {
    code = program_step(p);

    if (code == CODE_NO_INPUT) {
      queue_enqueue(p->input, &joystick);
    }

    if (array_size(p->output) >= 3) {
      value_t x = *(value_t *)array_get_ref(p->output, 0);
      value_t y = *(value_t *)array_get_ref(p->output, 1);
      value_t tile = *(value_t *)array_get_ref(p->output, 2);

      if (x == -1 && y == 0) {
        score = tile;
      } else {
        if (tile == TILE_PADDLE) {
          paddle_x = x;
        } else if (tile == TILE_BALL) {
          ball_x = x;
        }

        if (ball_x > paddle_x) {
          joystick = 1;
        } else if (ball_x < paddle_x) {
          joystick = -1;
        } else {
          joystick = 0;
        }
      }

      array_clear(p->output);
    }
  }

  return score;
}

void day13_solve(char *input, char *output) {
  char *input_copy = strdup(input);
  struct program *p = program_new();
  parse_input(input, p);

  int count = part1(p);

  program_destroy(p);

  p = program_new();
  parse_input(input_copy, p);

  int score = part2(p);

  sprintf(output, "Day13\nPart1: %d\nPart2: %d\n", count, score);

  program_destroy(p);
  free(input_copy);
}
