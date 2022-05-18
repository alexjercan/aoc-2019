#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/array.h"
#include "../include/intcode.h"
#include "../include/queue.h"
#include "../include/vector.h"

#define OPCODE_ADD 1
#define OPCODE_MULTIPLY 2
#define OPCODE_INPUT 3
#define OPCODE_OUTPUT 4
#define OPCODE_JUMP_IF_TRUE 5
#define OPCODE_JUMP_IF_FALSE 6
#define OPCODE_LESS_THAN 7
#define OPCODE_EQUALS 8
#define OPCODE_ADJUST_RELATIVE_BASE 9
#define OPCODE_HALT 99

#define MODE_POSITION 0
#define MODE_IMMEDIATE 1
#define MODE_RELATIVE 2

#define OUTPUT_SIZE 128
#define MEMORY_SIZE 2048

static value_t get_param(struct program *p, int index);
static void set_param(struct program *p, int index, value_t value);
static addr_t get_addr(struct program *p, int index);

struct program *program_new() {
  struct program *p = malloc(sizeof(struct program));
  p->ip = 0;
  p->relative_base = 0;
  p->memory = vector_new(MEMORY_SIZE, sizeof(value_t));
  p->input = queue_new(sizeof(value_t));
  p->output = array_new(OUTPUT_SIZE, sizeof(value_t));

  return p;
}

void program_copy(struct program *dst, struct program *src) {
  dst->ip = src->ip;
  dst->relative_base = src->relative_base;

  vector_copy(dst->memory, src->memory);
  queue_copy(dst->input, src->input);
  array_copy(dst->output, src->output);
}

int program_step(struct program *p) {
  if (p->ip >= vector_capacity(p->memory)) {
    return CODE_TAPE_END;
  }

  value_t opcode = *(value_t *)vector_get_ref(p->memory, p->ip) % 100;

  switch (opcode) {
  case OPCODE_ADD: {
    set_param(p, 3, get_param(p, 1) + get_param(p, 2));
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_MULTIPLY: {
    set_param(p, 3, get_param(p, 1) * get_param(p, 2));
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_INPUT: {
    if (queue_empty(p->input)) {
      return CODE_NO_INPUT;
    }
    value_t input;
    queue_dequeue(p->input, &input);
    set_param(p, 1, input);
    p->ip += 2;
    return CODE_OK;
  }
  case OPCODE_OUTPUT: {
    value_t output = get_param(p, 1);
    array_append(p->output, &output);
    p->ip += 2;
    return CODE_OUTPUT;
  }
  case OPCODE_JUMP_IF_TRUE: {
    p->ip = (get_param(p, 1) != 0) ? get_param(p, 2) : p->ip + 3;
    return CODE_OK;
  }

  case OPCODE_JUMP_IF_FALSE: {
    p->ip = (get_param(p, 1) == 0) ? get_param(p, 2) : p->ip + 3;
    return CODE_OK;
  }
  case OPCODE_LESS_THAN: {
    set_param(p, 3, (get_param(p, 1) < get_param(p, 2)));
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_EQUALS: {
    set_param(p, 3, (get_param(p, 1) == get_param(p, 2)));
    p->ip += 4;
    return CODE_OK;
  }
  case OPCODE_ADJUST_RELATIVE_BASE: {
    p->relative_base += get_param(p, 1);
    p->ip += 2;
    return CODE_OK;
  }
  case OPCODE_HALT:
    return CODE_HALT;
  default:
    printf("Unknown opcode: %lld\n", opcode);
    exit(1);
  }
}

void program_destroy(struct program *p) {
  vector_destroy(p->memory);
  queue_destroy(p->input);
  array_destroy(p->output);
  free(p);
}

static value_t get_param(struct program *p, int index) {
  return *(addr_t *)vector_get_ref(p->memory, get_addr(p, index));
}

static void set_param(struct program *p, int index, value_t value) {
  vector_set(p->memory, get_addr(p, index), &value);
}

static addr_t get_addr(struct program *p, int index) {
  value_t mode = *(value_t *)vector_get_ref(p->memory, p->ip) / 100;
  for (int i = 1; i < index; i++) {
    mode /= 10;
  }
  mode %= 10;

  switch (mode) {
  case MODE_POSITION:
    return *(addr_t *)vector_get_ref(p->memory, p->ip + index);
  case MODE_IMMEDIATE:
    return p->ip + index;
  case MODE_RELATIVE:
    return p->relative_base +
           *(addr_t *)vector_get_ref(p->memory, p->ip + index);
  default:
    fprintf(stderr, "Unknown mode: %lld\n", mode);
    exit(1);
  }
}
