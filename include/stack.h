#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct stack;

struct stack *stack_new(size_t element_size);
void stack_copy(struct stack *dst, struct stack *src);

int stack_push(struct stack *s, void *data);
int stack_pop(struct stack *s, void *data);
void *stack_pop_ref(struct stack *s);
int stack_peek(struct stack *s, void *data);
int stack_empty(struct stack *s);
void stack_destroy(struct stack *s);

#endif // STACK_H
