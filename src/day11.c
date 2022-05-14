#include "../include/day11.h"
#include "../include/array.h"
#include "../include/hashmap.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int point_cmp(const void *a, const void *b, void *udata) {
  const struct point *point_a = a;
  const struct point *point_b = b;

  char key_a[128] = {0};
  sprintf(key_a, "%d,%d", point_a->x, point_a->y);
  char key_b[128] = {0};
  sprintf(key_b, "%d,%d", point_b->x, point_b->y);

  return strcmp(key_a, key_b);
}

static uint64_t point_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const struct point *point = item;

  char key[128] = {0};
  sprintf(key, "%d,%d", point->x, point->y);

  return hashmap_sip(key, strlen(key), seed0, seed1);
}

struct robot {
  int x;
  int y;
  char direction;
};

static value_t get_color(struct hashmap *map, struct robot robot) {
  struct point point = {.x = robot.x, .y = robot.y};
  struct point *found = hashmap_get(map, &point);
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
  struct hashmap *map = hashmap_new(sizeof(struct point), 0, 0, 0, point_hash,
                                    point_cmp, NULL, NULL);
  int count = 0;
  int code = CODE_OK;
  struct robot robot = {.x = 0, .y = 0, .direction = '^'};
  while (true) {
    value_t input = get_color(map, robot);
    queue_enqueue(p->input, &input);

    while (array_size(p->output) < 2 && code == CODE_OK) {
      code = program_step(p);
    }

    if (code == CODE_HALT) {
      break;
    }

    value_t color = *(value_t *)array_get_ref(p->output, 0);
    value_t direction = *(value_t *)array_get_ref(p->output, 1);
    array_clear(p->output);

    struct point point = {.x = robot.x, .y = robot.y, .color = color};
    if (hashmap_get(map, &point) == NULL) {
      count++;
    }
    hashmap_set(map, &point);

    robot.direction = change_direction(robot.direction, direction);
    move_robot(&robot);
  }

  return count;
}

static int part2(struct program *p) {
  struct hashmap *map = hashmap_new(sizeof(struct point), 0, 0, 0, point_hash,
                                    point_cmp, NULL, NULL);

  int count = 0;
  int code = CODE_OK;
  struct robot robot = {.x = 0, .y = 0, .direction = '^'};
  struct point point = {.x = robot.x, .y = robot.y, .color = 1};
  hashmap_set(map, &point);
  while (true) {
    value_t input = get_color(map, robot);
    queue_enqueue(p->input, &input);

    while (array_size(p->output) < 2 && code == CODE_OK) {
      code = program_step(p);
    }

    if (code == CODE_HALT) {
      break;
    }

    value_t color = *(value_t *)array_get_ref(p->output, 0);
    value_t direction = *(value_t *)array_get_ref(p->output, 1);
    array_clear(p->output);

    struct point point = {.x = robot.x, .y = robot.y, .color = color};
    if (hashmap_get(map, &point) == NULL) {
      count++;
    }
    hashmap_set(map, &point);

    robot.direction = change_direction(robot.direction, direction);
    move_robot(&robot);
  }

  size_t index = 0;
  struct point **point_ptr = NULL;
  struct array *points = array_new(count, sizeof(struct point));
  while (hashmap_iter(map, &index, (void **)&point_ptr)) {
    array_append(points, point_ptr);
  }

  int minimum_x = 0;
  int maximum_x = 0;
  int minimum_y = 0;
  int maximum_y = 0;

  for (size_t i = 0; i < array_size(points); i++) {
    struct point point = *(struct point *)array_get_ref(points, i);
    if (point.x < minimum_x) {
      minimum_x = point.x;
    }
    if (point.x > maximum_x) {
      maximum_x = point.x;
    }
    if (point.y < minimum_y) {
      minimum_y = point.y;
    }
    if (point.y > maximum_y) {
      maximum_y = point.y;
    }
  }

  for (int y = minimum_y; y <= maximum_y; y++) {
    for (int x = minimum_x; x <= maximum_x; x++) {
      struct point point = {.x = x, .y = y};
      if (hashmap_get(map, &point) != NULL) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }

  return 0;
}

void day11_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day11\nPart1: %d\nPart2: %d\n", part1(p), part2(clone));
}
