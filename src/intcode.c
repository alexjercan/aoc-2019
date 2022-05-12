#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/util.h"

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

#define OUTPUT_SIZE 128
#define MEMORY_SIZE 2048

static int get_param(struct program *p, int index);

struct program *program_new() {
  struct program *p = malloc(sizeof(struct program));
  p->ip = 0;
  p->memory = array_new(MEMORY_SIZE, sizeof(int));
  p->input = queue_new(sizeof(int));
  p->output = array_new(OUTPUT_SIZE, sizeof(int));

  return p;
}

void program_copy(struct program *dst, struct program *src) {
  dst->ip = src->ip;

  array_copy(dst->memory, src->memory);
  queue_copy(dst->input, src->input);
  array_copy(dst->output, src->output);
}

int program_step(struct program *p) {
  if (p->ip >= array_size(p->memory)) {
    return CODE_TAPE_END;
  }

  int opcode = array_get_int(p->memory, p->ip) % 100;

  switch (opcode) {
  case OPCODE_ADD: {
    int dst = array_get_int(p->memory, p->ip + 3);
    int result = get_param(p, 1) + get_param(p, 2);
    array_set(p->memory, dst, &result);

    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_MULTIPLY: {
    int dst = array_get_int(p->memory, p->ip + 3);
    int result = get_param(p, 1) * get_param(p, 2);
    array_set(p->memory, dst, &result);
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_INPUT: {
    if (queue_empty(p->input)) {
      return CODE_NO_INPUT;
    }
    int input = queue_dequeue_int(p->input);
    int dst = array_get_int(p->memory, p->ip + 1);
    array_set(p->memory, dst, &input);
    p->ip += 2;
    return CODE_OK;
  }
  case OPCODE_OUTPUT: {
    int output = get_param(p, 1);
    array_append(p->output, &output);
    p->ip += 2;
    return CODE_OK;
  }
  case OPCODE_JUMP_IF_TRUE: {
    if (get_param(p, 1) != 0) {
      p->ip = get_param(p, 2);
    } else {
      p->ip += 3;
    }
    return CODE_OK;
  }

  case OPCODE_JUMP_IF_FALSE: {
    if (get_param(p, 1) == 0) {
      p->ip = get_param(p, 2);
    } else {
      p->ip += 3;
    }
    return CODE_OK;
  }
  case OPCODE_LESS_THAN: {
    int value = 0;
    if (get_param(p, 1) < get_param(p, 2)) {
      value = 1;
    }
    int dst = array_get_int(p->memory, p->ip + 3);
    array_set(p->memory, dst, &value);
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_EQUALS: {
    int value = 0;
    if (get_param(p, 1) == get_param(p, 2)) {
      value = 1;
    }
    int dst = array_get_int(p->memory, p->ip + 3);
    array_set(p->memory, dst, &value);
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_HALT:
    return CODE_HALT;
  default:
    printf("Unknown opcode: %d\n", opcode);
    exit(1);
  }
}

void program_destroy(struct program *p) {
  array_destroy(p->memory);
  queue_destroy(p->input);
  array_destroy(p->output);
  free(p);
}

static int get_param(struct program *p, int index) {
  int mode = array_get_int(p->memory, p->ip) / 100;
  for (int i = 1; i < index; i++) {
    mode /= 10;
  }
  mode %= 10;
  int value = array_get_int(p->memory, p->ip + index);

  if (mode == MODE_POSITION) {
    return array_get_int(p->memory, value);
  } else if (mode == MODE_IMMEDIATE) {
    return value;
  } else {
    fprintf(stderr, "Unknown mode: %d\n", mode);
    exit(1);
  }
}
