#include "../include/day14.h"
#include "../include/array.h"
#include "../include/hash.h"
#include "../include/hashmap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int amount_t;

struct chemical {
  char *name;
  amount_t amount;
};

static int chemical_cmp(const void *a, const void *b) {
  const struct chemical *node_a = a;
  const struct chemical *node_b = b;
  return strcmp(node_a->name, node_b->name);
}

static size_t chemical_hash(const void *item) {
  const struct chemical *node = item;
  return hash_sip(node->name, strlen(node->name), 0, 0);
}

struct reaction {
  struct array *inputs;
  struct chemical output;
};

static int reaction_cmp(const void *a, const void *b) {
  const struct reaction *node_a = a;
  const struct reaction *node_b = b;
  return strcmp(node_a->output.name, node_b->output.name);
}

static size_t reaction_hash(const void *item) {
  const struct reaction *node = item;
  return hash_sip(node->output.name, strlen(node->output.name), 0, 0);
}

static void parse_line(char *line, struct hashmap *recipes) {
  char *lhs = line;
  char *rhs = strstr(line, "=> ") + 3;

  struct array *lhs_chemicals = array_new(128, sizeof(struct chemical));
  char *token = strtok(lhs, " ");
  while (strcmp(token, "=>") != 0) {
    char *name = strtok(NULL, ", ");
    array_append(lhs_chemicals,
                 &(struct chemical){.name = name, .amount = atol(token)});

    token = strtok(NULL, " ");
  }

  token = strtok(rhs, " ");
  char *name = strtok(NULL, "\n");
  hashmap_set(recipes, &(struct reaction){
                           .inputs = lhs_chemicals,
                           .output = {.name = name, .amount = atoi(token)}});
}

static void parse_input(char *input, struct hashmap *recipes) {
  struct array *lines = array_new(128, sizeof(char *));
  char *line = strtok(input, "\n");

  while (line != NULL) {
    array_append(lines, &line);
    line = strtok(NULL, "\n");
  }

  for (size_t i = 0; i < array_size(lines); i++) {
    parse_line(*(char **)array_get_ref(lines, i), recipes);
  }

  array_destroy(lines);
}

static amount_t get_ore(struct hashmap *recipes, amount_t fuel) {
  struct array *recipes_array = array_new(128, sizeof(struct reaction));
  hashmap_to_array(recipes, recipes_array);

  struct hashmap *inventory =
      hashmap_new(128, sizeof(struct chemical), chemical_hash, chemical_cmp);
  for (size_t i = 0; i < array_size(recipes_array); i++) {
    struct reaction *reaction =
        *(struct reaction **)array_get_ref(recipes_array, i);
    hashmap_set(inventory,
                &(struct chemical){.name = reaction->output.name, .amount = 0});
  }
  hashmap_set(inventory, &(struct chemical){.name = "ORE", .amount = 0});
  hashmap_set(inventory,
              &(struct chemical){.name = "FUEL", .amount = -1 * fuel});

  struct array *inventory_array = array_new(128, sizeof(struct chemical));
  hashmap_to_array(inventory, inventory_array);

  int finished = 0;
  while (!finished) {
    int found = 0;
    for (int i = 0; i < array_size(inventory_array); i++) {
      struct chemical *chemical =
          *(struct chemical **)array_get_ref(inventory_array, i);

      if (strcmp(chemical->name, "ORE") == 0) {
        continue;
      } else if (chemical->amount < 0) {
        struct reaction *reaction = hashmap_get_ref(
            recipes, &(struct reaction){.output.name = chemical->name});
        amount_t multiplier =
            ceil((-1 * chemical->amount) / (float)reaction->output.amount);

        for (int j = 0; j < array_size(reaction->inputs); j++) {
          struct chemical *input =
              (struct chemical *)array_get_ref(reaction->inputs, j);
          struct chemical *inventory_chemical = hashmap_get_ref(
              inventory, &(struct chemical){.name = input->name});
          inventory_chemical->amount -= input->amount * multiplier;
        }
        chemical->amount += reaction->output.amount * multiplier;

        found = 1;
      }
    }

    if (!found) {
      finished = 1;
    }
  }

  struct chemical ore = *(struct chemical *)hashmap_get_ref(
      inventory, &(struct chemical){.name = "ORE"});

  array_destroy(inventory_array);
  hashmap_destroy(inventory);
  array_destroy(recipes_array);

  return -1 * ore.amount;
}

static amount_t part1(struct hashmap *recipes) { return get_ore(recipes, 1); }

static amount_t binary_search(struct hashmap *recipes, amount_t lower,
                              amount_t upper, amount_t ore_target) {
  amount_t mid = (lower + upper) / 2;
  amount_t ore_amount = get_ore(recipes, mid);

  while (lower < upper) {
    if (ore_amount <= ore_target) {
      lower = mid;
    } else {
      upper = mid - 1;
    }

    mid = (lower + upper) / 2;
    ore_amount = get_ore(recipes, mid);
  }

  return mid;
}

static amount_t part2(struct hashmap *recipes) {
  amount_t ore_target = 1000000000000;
  amount_t ore_for_one_fuel = get_ore(recipes, 1);
  amount_t fuel_count = ore_target / ore_for_one_fuel;

  return binary_search(recipes, fuel_count, fuel_count * 2, ore_target);
}

void day14_solve(char *input, char *output) {
  struct hashmap *recipes =
      hashmap_new(128, sizeof(struct reaction), reaction_hash, reaction_cmp);
  parse_input(input, recipes);

sprintf(output, "Day14\nPart1: %lld\nPart2: %lld\n", part1(recipes),
        part2(recipes));

  struct array *recipes_array = array_new(128, sizeof(struct reaction));
  hashmap_to_array(recipes, recipes_array);
  for (int i = 0; i < array_size(recipes_array); i++) {
    struct reaction *reaction =
        *(struct reaction **)array_get_ref(recipes_array, i);
    array_destroy(reaction->inputs);
  }
  array_destroy(recipes_array);
  hashmap_destroy(recipes);
}
