#ifndef INTCODE_H
#define INTCODE_H

struct program {
  int ip;
  struct array *memory;
  struct queue *input;
  struct array *output;
};

struct program *program_new();
struct program *program_clone(struct program *p);

int program_step(struct program *p);

void program_destroy(struct program *p);

#endif // INTCODE_H
