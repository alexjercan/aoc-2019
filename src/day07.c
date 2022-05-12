#include "../include/day07.h"
#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_AMPS 5

static void parse_input(char *input, struct program *p) {
  char *line = strtok(input, ",");
  while (line != NULL) {
    int data = atoi(line);

    array_append(p->memory, &data);
    line = strtok(NULL, ",");
  }
}

static void backtrack_permutations(int k, int n, struct array *permutations,
                                   struct array *permutation) {
  if (k == n) {
    struct array *new_permutation = array_new(n, sizeof(int));
    array_copy(new_permutation, permutation);
    array_append(permutations, &new_permutation);
    return;
  }

  for (int i = 0; i < n; i++) {
    if (array_contains(permutation, &i)) {
      continue;
    }

    array_append(permutation, &i);
    backtrack_permutations(k + 1, n, permutations, permutation);
    array_pop(permutation, NULL);
  }
}

struct array *permutations(int n) {
  struct array *perms = array_new(1, sizeof(struct array *));

  struct array *permutation = array_new(n, sizeof(int));

  backtrack_permutations(0, n, perms, permutation);

  array_destroy(permutation);

  return perms;
}

static int run_amp(struct program *p, int *input) {
  queue_enqueue(p->input, input);

  int code = 0;

  while (code == 0) {
    code = program_step(p);
  }

  *input = array_get_int(p->output, array_size(p->output) - 1);

  return code;
}

static int run_amps(struct program *p, struct array *phases) {
  int output = 0;

  for (int i = 0; i < array_size(phases); i++) {
    struct array *phase = NULL;
    array_get(phases, i, &phase);

    int input = 0;
    for (int j = 0; j < array_size(phase); j++) {
      struct program *amp = program_new();
      program_copy(amp, p);
      int phase_value = array_get_int(phase, j);
      queue_enqueue(amp->input, &phase_value);
      run_amp(amp, &input);
      program_destroy(amp);
    }

    if (input > output) {
      output = input;
    }
  }

  return output;
}

static int run_amps_loop(struct program *p, struct array *phases) {
  int output = 0;

  for (int i = 0; i < array_size(phases); i++) {
    struct array *phase = NULL;
    array_get(phases, i, &phase);

    struct array *amps = array_new(NUM_AMPS, sizeof(struct program *));
    for (int j = 0; j < NUM_AMPS; j++) {
      int phase_value = array_get_int(phase, j) + 5;
      struct program *amp = program_new();
      program_copy(amp, p);
      queue_enqueue(amp->input, &phase_value);
      array_append(amps, &amp);
    }

    int input = 0;
    int code = 0;
    while (code != 1) {
      for (int j = 0; j < NUM_AMPS; j++) {
        struct program *amp = NULL;
        array_get(amps, j, &amp);
        code = run_amp(amp, &input);
        array_set(amps, j, &amp);
      }
    }

    if (input > output) {
      output = input;
    }

    for (int j = 0; j < NUM_AMPS; j++) {
      struct program *amp = NULL;
      array_get(amps, j, &amp);
      program_destroy(amp);
    }
    array_destroy(amps);
  }

  return output;
}

static int part1(struct program *p) {
  struct array *perms = permutations(NUM_AMPS);

  int result = run_amps(p, perms);

  for (int i = 0; i < array_size(perms); i++) {
    struct array *perm = NULL;
    array_get(perms, i, &perm);
    array_destroy(perm);
  }
  array_destroy(perms);

  return result;
}

static int part2(struct program *p) {
  struct array *perms = permutations(NUM_AMPS);

  int result = run_amps_loop(p, perms);

  for (int i = 0; i < array_size(perms); i++) {
    struct array *perm = NULL;
    array_get(perms, i, &perm);
    array_destroy(perm);
  }
  array_destroy(perms);

  return result;
}

void day07_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day07\nPart1: %d\nPart2: %d\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
