#include "../include/day11.h"
#include "../include/array.h"
#include "../include/hash.h"
#include "../include/hashmap.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 2048

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

struct point {
  int x;
  int y;
  value_t color;
};

static int point_cmp(const void *a, const void *b) {
  const struct point *point_a = a;
  const struct point *point_b = b;

  char key_a[128] = {0};
  sprintf(key_a, "%d,%d", point_a->x, point_a->y);
  char key_b[128] = {0};
  sprintf(key_b, "%d,%d", point_b->x, point_b->y);

  return strcmp(key_a, key_b) == 0;
}

static size_t point_hash(const void *item) {
  const struct point *point = item;

  char key[128] = {0};
  sprintf(key, "%d,%d", point->x, point->y);

  return hash_sip(key, strlen(key), 0, 1);
}

struct robot {
  int x;
  int y;
  char direction;
};

static value_t get_color(struct hashmap *map, struct robot robot) {
  struct point point = {.x = robot.x, .y = robot.y};
  struct point *found = hashmap_get_ref(map, &point);
  if (found) {
    return found->color;
  }

  return 0;
}

static char change_direction(char direction, int side) {
  switch (direction) {
  case '^':
    if (side == 0) {
      return '<';
    } else {
      return '>';
    }
  case '>':
    if (side == 0) {
      return '^';
    } else {
      return 'v';
    }
  case 'v':
    if (side == 0) {
      return '>';
    } else {
      return '<';
    }
  case '<':
    if (side == 0) {
      return 'v';
    } else {
      return '^';
    }
  }

  return '^';
}

static void move_robot(struct robot *robot) {
  switch (robot->direction) {
  case '^':
    robot->y--;
    break;
  case '>':
    robot->x++;
    break;
  case 'v':
    robot->y++;
    break;
  case '<':
    robot->x--;
    break;
  }
}

static int part1(struct program *p) {
  struct hashmap *map =
      hashmap_new(CAPACITY, sizeof(struct point), point_hash, point_cmp);
  int count = 0;
  int code = CODE_OK;
  struct robot robot = {.x = 0, .y = 0, .direction = '^'};
  while (1) {
    value_t input = get_color(map, robot);
    queue_enqueue(p->input, &input);

    while (array_size(p->output) < 2 && code != CODE_HALT) {
      code = program_step(p);
    }

    if (code == CODE_HALT) {
      break;
    }

    value_t color = *(value_t *)array_get_ref(p->output, 0);
    value_t direction = *(value_t *)array_get_ref(p->output, 1);
    array_clear(p->output);

    struct point point = {.x = robot.x, .y = robot.y, .color = color};
    if (hashmap_get_ref(map, &point) == NULL) {
      count++;
    }
    hashmap_set(map, &point);

    robot.direction = change_direction(robot.direction, direction);
    move_robot(&robot);
  }

  hashmap_destroy(map);
  return count;
}

static char *part2(struct program *p) {
  value_t map[6][43] = {{0}};
  int code = CODE_OK;
  struct robot robot = {.x = 0, .y = 0, .direction = '^'};
  map[robot.y][robot.x] = 1;

  while (1) {
    value_t input = map[robot.y][robot.x];
    queue_enqueue(p->input, &input);

    while (array_size(p->output) < 2 && code != CODE_HALT) {
      code = program_step(p);
    }

    if (code == CODE_HALT) {
      break;
    }

    value_t color = *(value_t *)array_get_ref(p->output, 0);
    value_t direction = *(value_t *)array_get_ref(p->output, 1);
    array_clear(p->output);

    map[robot.y][robot.x] = color;

    robot.direction = change_direction(robot.direction, direction);
    move_robot(&robot);
  }

  char *output = calloc(6 * 44 + 1, sizeof(char));
  int index = 0;
  for (int y = 0; y <= 5; y++) {
    for (int x = 0; x <= 42; x++) {
      if (map[y][x] == 1) {
        output[index++] = '#';
      } else {
        output[index++] = ' ';
      }
    }
    output[index++] = '\n';
  }

  return output;
}

void day11_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  char *part2_output = part2(clone);
  sprintf(output, "Day11\nPart1: %d\nPart2: \n%s", part1(p), part2_output);

  free(part2_output);
  program_destroy(clone);
  program_destroy(p);
}
