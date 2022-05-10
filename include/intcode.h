#ifndef INTCODE_H
#define INTCODE_H

#define MEMORY_SIZE 2048

struct program {
  int ip;
  int memory[MEMORY_SIZE];
};

int intcode_step(struct program *p);

#endif // INTCODE_H
