#include "../include/day17.h"
#include "../include/array.h"
#include "../include/hashmap.h"
#include "../include/intcode.h"
#include "../include/tuple.h"
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

static void build_map(struct array *output, struct hashmap *map) {
  int x = 0, y = 0;

  for (int i = 0; i < array_size(output); i++) {
    int c = *(int *)array_get_ref(output, i);

    if (c == 10) {
      y++;
      x = 0;
    } else {
      if (c == 35) {
        struct tuple t = {.x = x, .y = y};
        hashmap_set(map, &t);
      }
      x++;
    }
  }
}

static int has_neighs(struct hashmap *map, struct tuple *t) {
  struct tuple tl = {.x = t->x - 1, .y = t->y};
  struct tuple tr = {.x = t->x + 1, .y = t->y};
  struct tuple tu = {.x = t->x, .y = t->y - 1};
  struct tuple td = {.x = t->x, .y = t->y + 1};

  return hashmap_get_ref(map, &tl) != NULL &&
         hashmap_get_ref(map, &tr) != NULL &&
         hashmap_get_ref(map, &td) != NULL && hashmap_get_ref(map, &tu) != NULL;
}

static value_t part1(struct program *p) {
  while (program_step(p) != CODE_HALT)
    ;

  struct hashmap *map =
      hashmap_new(1024, sizeof(struct tuple), tuple_hash, tuple_equals);
  build_map(p->output, map);

  struct array *points = array_new(1024, sizeof(struct tuple));
  hashmap_to_array(map, points);

  int result = 0;
  for (int i = 0; i < array_size(points); i++) {
    struct tuple *t = *(struct tuple **)array_get_ref(points, i);

    if (has_neighs(map, t)) {
      result += t->x * t->y;
    }
  }

  array_destroy(points);
  hashmap_destroy(map);

  return result;
}

static value_t part2(struct program *p) { return 0; }

void day17_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day17\nPart1: %lld\nPart2: %lld\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
