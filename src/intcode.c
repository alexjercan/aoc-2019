#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/intcode.h"

#define OPCODE_ADD 1
#define OPCODE_MULTIPLY 2
#define OPCODE_INPUT 3
#define OPCODE_OUTPUT 4
#define OPCODE_JUMP_IF_TRUE 5
#define OPCODE_JUMP_IF_FALSE 6
#define OPCODE_LESS_THAN 7
#define OPCODE_EQUALS 8
#define OPCODE_HALT 99

#define MODE_POSITION 0
#define MODE_IMMEDIATE 1

struct program intcode_init(int *memory) {
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  memset(p.input, 0, sizeof(int) * INPUT_SIZE);
  memset(p.output, 0, sizeof(int) * OUTPUT_SIZE);
  p.ip = 0;
  p.input_index = 0;
  p.output_index = 0;

  return p;
}

struct program intcode_init_input(int *memory, int *input) {
  struct program p;
  memcpy(p.memory, memory, sizeof(int) * MEMORY_SIZE);
  memcpy(p.input, input, sizeof(int) * INPUT_SIZE);
  memset(p.output, 0, sizeof(int) * OUTPUT_SIZE);
  p.ip = 0;
  p.input_index = 0;
  p.output_index = 0;

  return p;
}

static int get_mode(int code, int param_index) {
  int mode = code / 100;
  for (int i = 1; i < param_index; i++) {
    mode /= 10;
  }
  return mode % 10;
}

static int get_param(struct program *p, int index) {
  int mode = get_mode(p->memory[p->ip], index);
  int value = p->memory[p->ip + index];

  if (mode == MODE_POSITION) {
    return p->memory[value];
  } else if (mode == MODE_IMMEDIATE) {
    return value;
  } else {
    fprintf(stderr, "Unknown mode: %d\n", mode);
    exit(1);
  }
}

int intcode_step(struct program *p) {
  int opcode = p->memory[p->ip] % 100;

  switch (opcode) {
  case OPCODE_ADD:
    p->memory[p->memory[p->ip + 3]] = get_param(p, 1) + get_param(p, 2);
    p->ip += 4;
    return 0;
  case OPCODE_MULTIPLY:
    p->memory[p->memory[p->ip + 3]] = get_param(p, 1) * get_param(p, 2);
    p->ip += 4;
    return 0;
  case OPCODE_INPUT:
    p->memory[p->memory[p->ip + 1]] = p->input[p->input_index++];
    p->ip += 2;
    return 0;
  case OPCODE_OUTPUT:
    p->output[p->output_index++] = get_param(p, 1);
    p->ip += 2;
    return 0;
  case OPCODE_JUMP_IF_TRUE:
    if (get_param(p, 1) != 0) {
      p->ip = get_param(p, 2);
    } else {
      p->ip += 3;
    }
    return 0;
  case OPCODE_JUMP_IF_FALSE:
    if (get_param(p, 1) == 0) {
      p->ip = get_param(p, 2);
    } else {
      p->ip += 3;
    }
    return 0;
  case OPCODE_LESS_THAN:
    p->memory[p->memory[p->ip + 3]] =
        (get_param(p, 1) < get_param(p, 2)) ? 1 : 0;
    p->ip += 4;
    return 0;
  case OPCODE_EQUALS:
    p->memory[p->memory[p->ip + 3]] =
        (get_param(p, 1) == get_param(p, 2)) ? 1 : 0;
    p->ip += 4;
    return 0;
  case OPCODE_HALT:
    return 1;
  default:
    printf("Unknown opcode: %d\n", opcode);
    exit(1);
  }
}
