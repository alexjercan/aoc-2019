#include "../include/day02.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/intcode.h"

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
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  p.ip = 0;
  p.memory[1] = 12;
  p.memory[2] = 2;

  while (intcode_step(&p) == 0)
    ;

  return p.memory[0];
}

static int exec(int *memory, int noun, int verb) {
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  p.ip = 0;
  p.memory[1] = noun;
  p.memory[2] = verb;

  while (intcode_step(&p) == 0)
    ;

  return p.memory[0];
}

static int part2(int *memory) {
  int noun, verb;
  for (noun = 0; noun < 100; noun++) {
    for (verb = 0; verb < 100; verb++) {
      if (exec(memory, noun, verb) == 19690720) {
        return 100 * noun + verb;
      }
    }
  }

  return -1;
}

void day02_solve(char *input, char *output) {
  int *memory = parse_input(input);

  sprintf(output, "Day02\nPart1: %d\nPart2: %d\n", part1(memory),
          part2(memory));

  free(memory);
}
