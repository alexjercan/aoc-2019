#include "../include/day06.h"
#include "../include/array.h"
#include "../include/hashmap.h"
#include "../include/queue.h"
#include "../include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 2048

struct graph_node {
  char *name;
  struct array *neighbors;
};

static int graph_node_cmp(const void *a, const void *b) {
  const struct graph_node *node_a = a;
  const struct graph_node *node_b = b;
  return strcmp(node_a->name, node_b->name);
}

static size_t graph_node_hash(const void *item){
  const struct graph_node *node = item;
  return hash_sip(node->name, strlen(node->name), 0, 0);
}

static void parse_input(char *input, struct hashmap *map) {
  char *src = strtok(input, ")");

  while (src != NULL) {
    char *dst = strtok(NULL, "\n");

    struct graph_node *node = hashmap_get_ref(map, &(struct graph_node){.name = src});
    if (node == NULL) {
      hashmap_set(map, &(struct graph_node){.name = src, .neighbors = array_new(1024, sizeof(char **))});
      node = hashmap_get_ref(map, &(struct graph_node){.name = src});
    }
    array_append(node->neighbors, &dst);

    src = strtok(NULL, ")");
  }
}

static void parse_input_2(char *input, struct hashmap *map) {
  char *src = strtok(input, ")");

  while (src != NULL) {
    char *dst = strtok(NULL, "\n");

    struct graph_node *node = hashmap_get_ref(map, &(struct graph_node){.name = src});
    if (node == NULL) {
      hashmap_set(
          map, &(struct graph_node){
                   .name = src, .neighbors = array_new(1024, sizeof(char **))});
      node = hashmap_get_ref(map, &(struct graph_node){.name = src});
    }
    array_append(node->neighbors, &dst);

    node = hashmap_get_ref(map, &(struct graph_node){.name = dst});
    if (node == NULL) {
      hashmap_set(
          map, &(struct graph_node){
                   .name = dst, .neighbors = array_new(1024, sizeof(char **))});
      node = hashmap_get_ref(map, &(struct graph_node){.name = dst});
    }
    array_append(node->neighbors, &src);

    src = strtok(NULL, ")");
  }
}

struct queue_element {
  char *key;
  int level;
};

static int bfs(struct hashmap *map, char *src) {
  struct queue *queue = queue_new(sizeof(struct queue_element));
  struct queue_element element = {.key = src, .level = 0};
  queue_enqueue(queue, &element);

  int sum = 0;
  while (!queue_empty(queue)) {
    struct queue_element element;
    queue_dequeue(queue, &element);

    sum += element.level;
    struct graph_node *node =
        hashmap_get_ref(map, &(struct graph_node){.name = element.key});
    if (node != NULL) {
      for (int i = 0; i < array_size(node->neighbors); i++) {
        char *neighbor = NULL;
        array_get(node->neighbors, i, &neighbor);

        struct queue_element new_element = {.key = neighbor,
                                            .level = element.level + 1};
        queue_enqueue(queue, &new_element);
      }
    }
  }

  queue_destroy(queue);
  return sum;
}

struct visited_node {
  char *name;
  char *parent;
};

static int visited_node_cmp(const void *a, const void *b) {
  const struct visited_node *node_a = a;
  const struct visited_node *node_b = b;
  return strcmp(node_a->name, node_b->name);
}

static size_t visited_node_hash(const void *item) {
  const struct visited_node *node = item;
  return hash_sip(node->name, strlen(node->name), 0, 0);
}

static int bfs_path(struct hashmap *map, char *src, char *dst) {
  struct queue *queue = queue_new(sizeof(char *));
  queue_enqueue(queue, &src);

  struct hashmap *visited = hashmap_new(CAPACITY, sizeof(struct visited_node), visited_node_hash, visited_node_cmp);

  hashmap_set(visited, &(struct visited_node){.name = src, .parent = NULL});

  while (!queue_empty(queue)) {
    char *element = NULL;
    queue_dequeue(queue, &element);

    if (strcmp(element, dst) == 0) {
      break;
    }

    struct graph_node *node = hashmap_get_ref(map, &(struct graph_node){.name = element});
    if (node != NULL) {
      for (int i = 0; i < array_size(node->neighbors); i++) {
        char *neighbor = NULL;
        array_get(node->neighbors, i, &neighbor);

        struct visited_node *visited_node =
            hashmap_get_ref(visited, &(struct visited_node){.name = neighbor});
        if (visited_node == NULL) {
          queue_enqueue(queue, &neighbor);
          hashmap_set(visited, &(struct visited_node){.name = neighbor,
                                                      .parent = element});
        }
      }
    }
  }

  int path_length = 0;
  struct visited_node *iter =
      hashmap_get_ref(visited, &(struct visited_node){.name = dst});
  while (iter != NULL && iter->parent != NULL) {
    iter = hashmap_get_ref(visited, &(struct visited_node){.name = iter->parent});
    path_length++;
  }

  hashmap_destroy(visited);
  queue_destroy(queue);
  return path_length - 2;
}

static int part1(struct hashmap *map) { return bfs(map, "COM"); }

static int part2(struct hashmap *map) { return bfs_path(map, "YOU", "SAN"); }

void day06_solve(char *input, char *output) {
  char *input_2 = strdup(input);
  struct hashmap *map = hashmap_new(CAPACITY, sizeof(struct graph_node), graph_node_hash, graph_node_cmp);
  parse_input(input, map);
  struct hashmap *undirected_map = hashmap_new(CAPACITY, sizeof(struct graph_node), graph_node_hash, graph_node_cmp);
  parse_input_2(input_2, undirected_map);

  sprintf(output, "Day06\nPart1: %d\nPart2: %d\n", part1(map),
          part2(undirected_map));

  hashmap_destroy(map);
  hashmap_destroy(undirected_map);

  free(input_2);
}
