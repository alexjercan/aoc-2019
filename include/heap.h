#ifndef HEAP_H
#define HEAP_H

struct heap;

struct heap *heap_new(int capacity, int element_size,
                      int (*compare)(const void *, const void *));
int heap_push(struct heap *heap, void *data);
int heap_pop(struct heap *heap, void *data);
void *heap_peek(struct heap *heap);
int heap_empty(struct heap *heap);
void heap_destroy(struct heap *heap);

#endif // HEAP_H
