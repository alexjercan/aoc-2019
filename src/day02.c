#include "../include/day02.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/intcode.h"

int *day02_parse_input(char *input) {
  int *memory = calloc(sizeof(int), MEMORY_SIZE);

  int index = 0;
  char *line = strtok(input, ",");
  while (line != NULL) {
    int value = atoi(line);
    int *data = malloc(sizeof(int));
    *data = value;
    memory[index] = value;
    index++;
    line = strtok(NULL, ",");
  }

  return memory;
}

int day02_part1(int *memory) {
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  p.ip = 0;
  p.memory[1] = 12;
  p.memory[2] = 2;

  while (intcode_step(&p) == 0)
    ;

  return p.memory[0];
}

int day02_exec(int *memory, int noun, int verb) {
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  p.ip = 0;
  p.memory[1] = noun;
  p.memory[2] = verb;

  while (intcode_step(&p) == 0)
    ;

  return p.memory[0];
}

int day02_part2(int *memory) {
  int noun, verb;
  for (noun = 0; noun < 100; noun++) {
    for (verb = 0; verb < 100; verb++) {
      if (day02_exec(memory, noun, verb) == 19690720) {
        return 100 * noun + verb;
      }
    }
  }

  return -1;
}

void day02_solve(char *input, char *output) {
  int *memory = day02_parse_input(input);

  sprintf(output, "Day02\nPart1: %d\nPart2: %d\n", day02_part1(memory),
          day02_part2(memory));

  free(memory);
}
