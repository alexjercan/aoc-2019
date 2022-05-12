#ifndef INTCODE_H
#define INTCODE_H

#define CODE_OK 0
#define CODE_HALT 1
#define CODE_TAPE_END 2
#define CODE_NO_INPUT 3

struct program {
  int ip;
  struct array *memory;
  struct queue *input;
  struct array *output;
};

struct program *program_new();
void program_copy(struct program *dst, struct program *src);

int program_step(struct program *p);

void program_destroy(struct program *p);

#endif // INTCODE_H
