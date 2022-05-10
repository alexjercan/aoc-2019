#ifndef INTCODE_H
#define INTCODE_H

#define INPUT_SIZE 128
#define OUTPUT_SIZE 128
#define MEMORY_SIZE 2048

struct program {
  int ip;
  int memory[MEMORY_SIZE];
  int input_index;
  int input[INPUT_SIZE];
  int output_index;
  int output[OUTPUT_SIZE];
};

struct program intcode_init(int *memory);
struct program intcode_init_input(int *memory, int *input);
int intcode_step(struct program *p);

#endif // INTCODE_H
