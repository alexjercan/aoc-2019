#include "../include/day09.h"
#include "../include/array.h"
#include "../include/intcode.h"

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

void day09_solve(char *input, char *output) {
  sprintf(output, "Day09\nPart1: %d\nPart2: %d\n", 0, 0);
}
