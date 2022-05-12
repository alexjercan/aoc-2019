#include "../include/day01.h"
#include "../include/array.h"
#include "../include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void parse_input(char *input, struct array *array) {
  char *line = strtok(input, "\n");
  while (line != NULL) {
    int value = atoi(line);
    array_append(array, &value);
    line = strtok(NULL, "\n");
  }
}

static int part1(struct array *array) {
  int sum = 0;

  for (int i = 0; i < array_size(array); i++) {
    sum += array_get_int(array, i) / 3 - 2;
  }

  return sum;
}

static int part2(struct array *array) {
  int sum = 0;

  for (int i = 0; i < array_size(array); i++) {
    int fuel = array_get_int(array, i) / 3 - 2;
    while (fuel > 0) {
      sum += fuel;
      fuel = fuel / 3 - 2;
    }
  }

  return sum;
}

void day01_solve(char *input, char *output) {
  struct array *array = array_new(1024, sizeof(int));
  parse_input(input, array);

  sprintf(output, "Day01\nPart1: %d\nPart2: %d\n", part1(array), part2(array));

  array_destroy(array);
}
