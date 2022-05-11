#include "../include/day02.h"
#include "../include/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/intcode.h"

static void parse_input(char *input, struct program *p) {
  char *line = strtok(input, ",");
  while (line != NULL) {
    int data = atoi(line);

    array_append(p->memory, &data);
    line = strtok(NULL, ",");
  }
}

static int array_get_value(struct array *m, int index) {
    int value = 0;
    array_get(m, index, &value);
    return value;
}

static int part1(struct program *p) {
  int a = 12, b = 2;
  array_set(p->memory, 1, &a);
  array_set(p->memory, 2, &b);

  while (program_step(p) == 0)
    ;

  return array_get_value(p->memory, 0);
}

static int exec(struct program *p, int noun, int verb) {
  array_set(p->memory, 1, &noun);
  array_set(p->memory, 2, &verb);

  while (program_step(p) == 0)
    ;

  return array_get_value(p->memory, 0);
}

static int part2(struct program *p) {
  int noun, verb;
  for (noun = 0; noun < 100; noun++) {
    for (verb = 0; verb < 100; verb++) {
      struct program *clone = program_clone(p);
      int result = exec(clone, noun, verb);
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

  struct program *clone = program_clone(p);

  sprintf(output, "Day02\nPart1: %d\nPart2: %d\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
