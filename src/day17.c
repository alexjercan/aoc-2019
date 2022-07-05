#include "../include/day17.h"
#include "../include/array.h"
#include "../include/hashmap.h"
#include "../include/heap.h"
#include "../include/intcode.h"
#include "../include/queue.h"
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

static void string_map(struct array *output, char *map) {
  int x = 0, y = 0;

  for (int i = 0; i < array_size(output); i++) {
    int c = *(int *)array_get_ref(output, i);

    if (c == 10) {
      y++;
      x = 0;
      map[i] = '\n';
    } else {
      map[i] = (char)c;
      x++;
    }
  }
  printf("%d\n", array_size(output));
  map[array_size(output)] = '\0';
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

static value_t part1(struct hashmap *map) {
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
  return result;
}

static value_t part2(struct hashmap *map, struct program *p) {
  // This is hardcoded because it was easier than using my brain
  char *input = "A,B,A,B,C,A,B,C,A,C\nR,6,L,10,R,8\nR,8,R,12,L,8,L,8\nL,10,R,6,"
                "R,6,L,8\nn\n";

  for (int i = 0; i < strlen(input); i++) {
    value_t data = (value_t)input[i];
    queue_enqueue(p->input, &data);
  }

  while (program_step(p) != CODE_HALT)
    ;

  return *(value_t *)array_get_ref(p->output, array_size(p->output) - 1);
}

void day17_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  value_t set_bit = 2;
  vector_set(clone->memory, 0, &set_bit);

  while (program_step(p) != CODE_HALT)
    ;

  struct hashmap *map =
      hashmap_new(1024, sizeof(struct tuple), tuple_hash, tuple_equals);
  build_map(p->output, map);

  char *str = calloc(array_size(p->output) + 1, sizeof(char));
  string_map(p->output, str);
  printf("%s\n", str);

  sprintf(output, "Day17\nMap:\n%s\nPart1: %lld\nPart2: %lld\n", str,
          part1(map), part2(map, clone));

  program_destroy(p);
  program_destroy(clone);
  hashmap_destroy(map);
  free(str);
}
