#include "../include/day05.h"
#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int input = 1;
  queue_enqueue(p->input, &input);

  while (program_step(p) == 0)
    ;

  return array_get_value(p->output, array_size(p->output) - 1);
}

static int part2(struct program *p) {
  int input = 5;
  queue_enqueue(p->input, &input);

  while (program_step(p) == 0)
    ;

  return array_get_value(p->output, array_size(p->output) - 1);
}

void day05_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_clone(p);

  sprintf(output, "Day05\nPart1: %d\nPart2: %d\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
