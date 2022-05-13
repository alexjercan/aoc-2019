#include "../include/day02.h"
#include "../include/intcode.h"
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

static value_t part1(struct program *p) {
  value_t a = 12, b = 2;
  vector_set(p->memory, 1, &a);
  vector_set(p->memory, 2, &b);

  while (program_step(p) == 0)
    ;

  return *(value_t *)vector_get_ref(p->memory, 0);
}

static value_t exec(struct program *p, value_t noun, value_t verb) {
  vector_set(p->memory, 1, &noun);
  vector_set(p->memory, 2, &verb);

  while (program_step(p) == 0)
    ;

  return *(value_t *)vector_get_ref(p->memory, 0);
}

static value_t part2(struct program *p) {
  for (value_t noun = 0; noun < 100; noun++) {
    for (value_t verb = 0; verb < 100; verb++) {
      struct program *clone = program_new();
      program_copy(clone, p);
      value_t result = exec(clone, noun, verb);
      program_destroy(clone);

      if (result == 19690720) {
        return 100 * noun + verb;
      }
    }
  }

  return -1;
}

void day02_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day02\nPart1: %lld\nPart2: %lld\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
