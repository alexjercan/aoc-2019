#include <stdio.h>
#include <stdlib.h>

#include "../include/intcode.h"

#define OPCODE_ADD 1
#define OPCODE_MULTIPLY 2
#define OPCODE_HALT 99

int intcode_step(struct program *p) {
  int opcode = p->memory[p->ip];

  switch (opcode) {
  case OPCODE_ADD:
    p->memory[p->memory[p->ip + 3]] =
        p->memory[p->memory[p->ip + 1]] + p->memory[p->memory[p->ip + 2]];
    p->ip += 4;
    return 0;
  case OPCODE_MULTIPLY:
    p->memory[p->memory[p->ip + 3]] =
        p->memory[p->memory[p->ip + 1]] * p->memory[p->memory[p->ip + 2]];
    p->ip += 4;
    return 0;
  case OPCODE_HALT:
    return 1;
  default:
    printf("Unknown opcode: %d\n", opcode);
    exit(1);
  }
}
