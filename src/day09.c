#include "../include/day09.h"
#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"
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
  value_t input = 1;
  queue_enqueue(p->input, &input);

  while (program_step(p) == 0)
    ;

  return *(value_t *)array_get_ref(p->output, array_size(p->output) - 1);
}

static value_t part2(struct program *p) {
  value_t input = 2;
  queue_enqueue(p->input, &input);

  while (program_step(p) == 0)
    ;

  return *(value_t *)array_get_ref(p->output, array_size(p->output) - 1);
}

void day09_solve(char *input, char *output) {
  struct program *p = program_new();
  parse_input(input, p);

  struct program *clone = program_new();
  program_copy(clone, p);

  sprintf(output, "Day09\nPart1: %lld\nPart2: %lld\n", part1(p), part2(clone));

  program_destroy(p);
  program_destroy(clone);
}
