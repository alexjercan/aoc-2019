#include "../include/day15.h"
#include "../include/array.h"
#include "../include/hash.h"
#include "../include/hashmap.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

#define STATUS_WALL 0
#define STATUS_MOVE 1
#define STATUS_GOAL 2

const static value_t directions[4] = {NORTH, SOUTH, WEST, EAST};
const static value_t counter_directions[4] = {SOUTH, NORTH, EAST, WEST};

struct point {
  int x;
  int y;
};

static size_t point_hash(const void *point) {
  const struct point *p = point;
  char buffer[100] = {0};
  sprintf(buffer, "%d,%d", p->x, p->y);
  return hash_sip(buffer, strlen(buffer), 0, 0);
}

static int point_equals(const void *a, const void *b) {
  const struct point *p1 = a;
  const struct point *p2 = b;
  return p1->x == p2->x && p1->y == p2->y;
}

struct point_kv {
  struct point key;
  struct point value;
};

static size_t point_vk_hash(const void *point_kv) {
  return point_hash(&(((struct point_kv *)point_kv)->key));
}

static int point_kv_equals(const void *a, const void *b) {
  const struct point_kv *p1 = a;
  const struct point_kv *p2 = b;
  return point_equals(&p1->key, &p2->key);
}

struct flood_kv {
  struct point key;
  int value;
};

static size_t flood_kv_hash(const void *point) {
  return point_hash(&(((struct flood_kv *)point)->key));
}

static int flood_kv_equals(const void *a, const void *b) {
  const struct flood_kv *p1 = a;
  const struct flood_kv *p2 = b;
  return point_equals(&p1->key, &p2->key);
}

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

static struct point move_direction(value_t dir, struct point p) {
  switch (dir) {
  case NORTH:
    p.y--;
    break;
  case SOUTH:
    p.y++;
    break;
  case WEST:
    p.x--;
    break;
  case EAST:
    p.x++;
    break;
  }

  return p;
}

static value_t run_program(struct program *p, value_t dir) {
  queue_enqueue(p->input, &dir);

  while (program_step(p) != CODE_OUTPUT)
    ;

  return *(value_t *)array_get_ref(p->output, array_size(p->output) - 1);
}

static void build_map(struct program *program, struct point droid,
                      struct array *points, struct point *goal) {
  for (int i = 0; i < 4; i++) {
    value_t dir = directions[i];
    value_t status = run_program(program, dir);

    if (status == STATUS_WALL) {
      continue;
    }

    if (status == STATUS_GOAL) {
      *goal = move_direction(dir, droid);
    }

    struct point p = move_direction(dir, droid);
    if (!array_contains(points, &p)) {
      array_append(points, &p);
      build_map(program, p, points, goal);
    }

    dir = counter_directions[i];
    run_program(program, dir);
  }
}

static int bfs(struct array *points, struct point start, struct point goal) {
  struct queue *q = queue_new(sizeof(struct point));
  queue_enqueue(q, &start);

  struct hashmap *visited =
      hashmap_new(1024, sizeof(struct point), point_hash, point_equals);
  hashmap_set(visited, &start);

  struct hashmap *parent = hashmap_new(1024, sizeof(struct point_kv),
                                       point_vk_hash, point_kv_equals);

  struct point p;
  while (!queue_empty(q)) {
    queue_dequeue(q, &p);

    if (point_equals(&p, &goal)) {
      break;
    }

    for (int i = 0; i < 4; i++) {
      struct point next = move_direction(directions[i], p);
      if (hashmap_get_ref(visited, &next) == NULL &&
          array_contains(points, &next)) {
        hashmap_set(visited, &next);
        queue_enqueue(q, &next);
        hashmap_set(parent, &(struct point_kv){.key = next, .value = p});
      }
    }
  }

  while (!queue_empty(q)) {
    queue_dequeue(q, &p);
  }
  queue_destroy(q);
  hashmap_destroy(visited);

  int steps = 0;
  p = goal;
  while (!point_equals(&p, &start)) {
    p = ((struct point_kv *)hashmap_get_ref(parent,
                                            &(struct point_kv){.key = p}))
            ->value;
    steps++;
  }

  hashmap_destroy(parent);

  return steps;
}

static int part1(struct program *p) {
  struct array *points = array_new(1024, sizeof(struct point));
  struct point goal = {0, 0};
  struct point current = {0, 0};
  array_append(points, &current);

  build_map(p, current, points, &goal);

  int result = bfs(points, current, goal);

  array_destroy(points);

  return result;
}

static int flood(struct array *points, struct point start) {
  struct queue *q = queue_new(sizeof(struct point));
  queue_enqueue(q, &start);

  struct hashmap *visited = hashmap_new(1024, sizeof(struct flood_kv),
                                        flood_kv_hash, flood_kv_equals);
  hashmap_set(visited, &(struct flood_kv){.key = start, .value = 0});

  struct point p;
  int max_value = 0;
  while (!queue_empty(q)) {
    queue_dequeue(q, &p);

    for (int i = 0; i < 4; i++) {
      struct point next = move_direction(directions[i], p);
      if (hashmap_get_ref(visited, &(struct flood_kv){.key = next}) == NULL &&
          array_contains(points, &next)) {
        struct flood_kv kv = *(struct flood_kv *)hashmap_get_ref(
            visited, &(struct flood_kv){.key = p});
        hashmap_set(visited,
                    &(struct flood_kv){.key = next, .value = kv.value + 1});
        queue_enqueue(q, &next);
        if (kv.value + 1 > max_value) {
          max_value = kv.value + 1;
        }
      }
    }
  }

  while (!queue_empty(q)) {
    queue_dequeue(q, &p);
  }
  queue_destroy(q);
  hashmap_destroy(visited);

  return max_value;
}

static int part2(struct program *p) {
  struct array *points = array_new(1024, sizeof(struct point));
  struct point goal = {0, 0};
  struct point current = {0, 0};
  array_append(points, &current);

  build_map(p, current, points, &goal);

  int result = flood(points, goal);

  array_destroy(points);

  return result;
}

void day15_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day15\nPart1: %d\nPart2: %d\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
