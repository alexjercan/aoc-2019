#include "../include/day06.h"
#include "../include/hashmap.h"
#include "../include/linked_list.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 2048

struct graph_node {
  char *name;
  struct linked_list *neighbors;
};

static int graph_node_cmp(const void *a, const void *b, void *udata) {
  const struct graph_node *node_a = a;
  const struct graph_node *node_b = b;
  return strcmp(node_a->name, node_b->name);
}

static uint64_t graph_node_hash(const void *item, uint64_t seed0,
                                uint64_t seed1) {
  const struct graph_node *node = item;
  return hashmap_sip(node->name, strlen(node->name), seed0, seed1);
}

static struct hashmap *parse_input(char *input) {
  struct hashmap *map =
      hashmap_new(sizeof(struct graph_node), 0, 0, 0, graph_node_hash,
                  graph_node_cmp, NULL, NULL);

  char *src = strtok(input, ")");

  while (src != NULL) {
    char *dst = strtok(NULL, "\n");

    struct graph_node *node =
        hashmap_get(map, &(struct graph_node){.name = src});
    if (node == NULL) {
      hashmap_set(map, &(struct graph_node){.name = src, .neighbors = NULL});
      node = hashmap_get(map, &(struct graph_node){.name = src});
    }
    linked_list_append(&node->neighbors, dst);

    src = strtok(NULL, ")");
  }

  return map;
}

static struct hashmap *parse_input_2(char *input) {
  struct hashmap *map =
      hashmap_new(sizeof(struct graph_node), 0, 0, 0, graph_node_hash,
                  graph_node_cmp, NULL, NULL);

  char *src = strtok(input, ")");

  while (src != NULL) {
    char *dst = strtok(NULL, "\n");

    struct graph_node *node =
        hashmap_get(map, &(struct graph_node){.name = src});
    if (node == NULL) {
      hashmap_set(map, &(struct graph_node){.name = src, .neighbors = NULL});
      node = hashmap_get(map, &(struct graph_node){.name = src});
    }
    linked_list_append(&node->neighbors, dst);

    node = hashmap_get(map, &(struct graph_node){.name = dst});
    if (node == NULL) {
      hashmap_set(map, &(struct graph_node){.name = dst, .neighbors = NULL});
      node = hashmap_get(map, &(struct graph_node){.name = dst});
    }
    linked_list_append(&node->neighbors, src);

    src = strtok(NULL, ")");
  }

  return map;
}

struct queue_element {
  char *key;
  int level;
};

static int bfs(struct hashmap *map, char *src) {
  struct linked_list *queue = NULL;
  struct queue_element *element = malloc(sizeof(struct queue_element));
  element->key = src;
  element->level = 0;
  linked_list_append(&queue, element);

  int sum = 0;
  while (queue != NULL) {
    struct queue_element *element = linked_list_pop_front(&queue);

    sum += element->level;
    struct graph_node *node =
        hashmap_get(map, &(struct graph_node){.name = element->key});
    if (node != NULL) {
      for (struct linked_list *neighbor = node->neighbors; neighbor != NULL;
           neighbor = neighbor->next) {
        struct queue_element *new_element =
            malloc(sizeof(struct queue_element));
        new_element->key = neighbor->data;
        new_element->level = element->level + 1;

        linked_list_append(&queue, new_element);
      }
    }

    free(element);
  }

  return sum;
}

struct visited_node {
  char *name;
  char *parent;
};

static int visited_node_cmp(const void *a, const void *b, void *udata) {
  const struct visited_node *node_a = a;
  const struct visited_node *node_b = b;
  return strcmp(node_a->name, node_b->name);
}

static uint64_t visited_node_hash(const void *item, uint64_t seed0,
                                  uint64_t seed1) {
  const struct visited_node *node = item;
  return hashmap_sip(node->name, strlen(node->name), seed0, seed1);
}

static int bfs_path(struct hashmap *map, char *src, char *dst) {
  struct linked_list *queue = NULL;
  linked_list_append(&queue, src);

  struct hashmap *visited =
      hashmap_new(sizeof(struct visited_node), 0, 0, 0, visited_node_hash,
                  visited_node_cmp, NULL, NULL);

  hashmap_set(visited, &(struct visited_node){.name = src, .parent = NULL});

  while (queue != NULL) {
    char *element = linked_list_pop_front(&queue);

    if (strcmp(element, dst) == 0) {
      break;
    }

    struct graph_node *node =
        hashmap_get(map, &(struct graph_node){.name = element});
    if (node != NULL) {
      for (struct linked_list *neighbor = node->neighbors; neighbor != NULL;
           neighbor = neighbor->next) {

        struct visited_node *visited_node = hashmap_get(
            visited, &(struct visited_node){.name = neighbor->data});
        if (visited_node == NULL) {
          linked_list_append(&queue, neighbor->data);
          hashmap_set(visited, &(struct visited_node){.name = neighbor->data,
                                                      .parent = element});
        }
      }
    }
  }

  while (queue != NULL) {
    linked_list_pop_front(&queue);
  }

  int path_length = 0;
  struct visited_node *iter =
      hashmap_get(visited, &(struct visited_node){.name = dst});
  while (iter != NULL && iter->parent != NULL) {
    iter = hashmap_get(visited, &(struct visited_node){.name = iter->parent});
    path_length++;
  }

  hashmap_free(visited);
  return path_length - 2;
}

static int part1(struct hashmap *map) { return bfs(map, "COM"); }

static int part2(struct hashmap *map) { return bfs_path(map, "YOU", "SAN"); }

void day06_solve(char *input, char *output) {
  char *input_2 = strdup(input);
  struct hashmap *map = parse_input(input);
  struct hashmap *undirected_map = parse_input_2(input_2);

  sprintf(output, "Day06\nPart1: %d\nPart2: %d\n", part1(map),
          part2(undirected_map));

  size_t iter = 0;
  void *item = NULL;
  while (hashmap_iter(map, &iter, &item)) {
    struct graph_node *node = item;
    linked_list_free(node->neighbors);
  }
  hashmap_free(map);

  iter = 0;
  item = NULL;
  while (hashmap_iter(undirected_map, &iter, &item)) {
    struct graph_node *node = item;
    linked_list_free(node->neighbors);
  }
  hashmap_free(undirected_map);

  free(input_2);
}
