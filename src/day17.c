#include "../include/day17.h"
#include "../include/array.h"
#include "../include/hashmap.h"
#include "../include/heap.h"
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

struct node {
  struct tuple t;
  int *cost;
  int id;
  int parent;
};

static int node_equals(const void *a, const void *b) {
  const struct node *n1 = a;
  const struct node *n2 = b;

  return tuple_equals(&n1->t, &n2->t);
};

static size_t node_hash(const void *a) {
  const struct node *n = a;

  return tuple_hash(&n->t);
};

struct heap_node {
  int id;
  int cost;
};

static int heap_node_compare(const void *a, const void *b) {
  const struct heap_node *n1 = a;
  const struct heap_node *n2 = b;

  return n1->cost - n2->cost;
};

static void dijkstra(struct hashmap *map) {
  struct array *points = array_new(1024, sizeof(struct tuple));
  hashmap_to_array(map, points);

    struct hashmap *cost = hashmap_new(1024, sizeof(struct node), node_hash, node_equals);
    struct heap *heap = heap_new(1024, sizeof(struct heap_node), heap_node_compare);

    for (int i = 0; i < array_size(points); i++) {
      struct tuple *t = *(struct tuple **)array_get_ref(points, i);
        int *c = calloc(2 << array_size(points), sizeof(int));
        c[2 << i] = 0;

        struct node n = {.t = *t, .cost = c, .id = i, .parent = -1};
        struct heap_node hn = {.id = i, .cost = 99999};
        hashmap_set(cost, &n);
        heap_push(heap, &hn);
    }

    while (!heap_empty(heap)) {


    }

}

static value_t part2(struct hashmap *map, struct program *p) {
  dijkstra(map);

  return 0;
}

void day17_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  while (program_step(p) != CODE_HALT)
    ;

  struct hashmap *map =
      hashmap_new(1024, sizeof(struct tuple), tuple_hash, tuple_equals);
  build_map(p->output, map);

  sprintf(output, "Day17\nPart1: %lld\nPart2: %lld\n", part1(map),
          part2(map, clone));

  program_destroy(p);
  program_destroy(clone);
  hashmap_destroy(map);
}
