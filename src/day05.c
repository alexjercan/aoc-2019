#include "../include/day05.h"
#include "../include/intcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *parse_input(char *input) {
  int *memory = calloc(sizeof(int), MEMORY_SIZE);

  int index = 0;
  char *line = strtok(input, ",");
  while (line != NULL) {
    int value = atoi(line);
    memory[index] = value;
    index++;
    line = strtok(NULL, ",");
  }

  return memory;
}

static int part1(int *memory) {
  int input[INPUT_SIZE] = {1};
  struct program p = intcode_init_input(memory, input);

  while (intcode_step(&p) == 0)
    ;

  return p.output[p.output_index - 1];
}

static int part2(int *memory) {
  int input[INPUT_SIZE] = {5};
  struct program p = intcode_init_input(memory, input);

  while (intcode_step(&p) == 0)
    ;

  return p.output[p.output_index - 1];
}

void day05_solve(char *input, char *output) {
  int *memory = parse_input(input);

  sprintf(output, "Day05\nPart1: %d\nPart2: %d\n", part1(memory),
          part2(memory));

  free(memory);
}
