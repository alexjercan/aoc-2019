#include "../include/stack.h"

#include <string.h>

struct node {
  void *data;
  struct node *next;
};

struct stack {
  struct node *top;
  size_t element_size;
};

struct stack *stack_new(size_t element_size) {
  struct stack *s = malloc(sizeof(struct stack));

  s->top = NULL;
  s->element_size = element_size;

  return s;
}

void stack_copy(struct stack *dst, struct stack *src) {
  struct node *n = src->top;

  dst->top = NULL;
  dst->element_size = src->element_size;

  while (n) {
    stack_push(dst, n->data);
    n = n->next;
  }
}

int stack_push(struct stack *s, void *data) {
  struct node *n = malloc(sizeof(struct node));
  n->data = malloc(s->element_size);
  memcpy(n->data, data, s->element_size);
  n->next = s->top;
  s->top = n;

  return 0;
}

int stack_pop(struct stack *s, void *data) {
  if (s->top == NULL) {
    return -1;
  }

  struct node *n = s->top;
  s->top = n->next;

  memcpy(data, n->data, s->element_size);
  free(n->data);
  free(n);

  return 0;
}

void *stack_pop_ref(struct stack *s) {
  void *data = malloc(s->element_size);
  stack_pop(s, data);
  return data;
}

int stack_peek(struct stack *s, void *data) {
  if (s->top == NULL) {
    return -1;
  }

  memcpy(data, s->top->data, s->element_size);

  return 0;
}

int stack_empty(struct stack *s) { return s->top == NULL; }

void stack_destroy(struct stack *s) {
  struct node *n = s->top;

  while (n) {
    struct node *next = n->next;
    free(n->data);
    free(n);
    n = next;
  }

  free(s);
}
