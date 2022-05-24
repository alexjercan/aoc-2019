#include "../include/heap.h"
#include "../include/array.h"

struct heap {
  struct array *array;
  int (*compare)(const void *, const void *);
};

struct heap *heap_new(int capacity, int element_size,
                      int (*compare)(const void *, const void *)) {
  struct heap *heap = malloc(sizeof(struct heap));
  heap->array = array_new(capacity, element_size);
  heap->compare = compare;

  return heap;
}

static void heapify_helper(struct heap *heap, int index) {
  int largest = index;
  int left = 2 * index + 1;
  int right = 2 * index + 2;

  void *largest_element = array_get_ref(heap->array, largest);
  void *left_element = array_get_ref(heap->array, left);
  void *right_element = array_get_ref(heap->array, right);

  if (left < array_size(heap->array) &&
      heap->compare(left_element, largest_element) > 0) {
    largest = left;
    largest_element = left_element;
  }

  if (right < array_size(heap->array) &&
      heap->compare(right_element, largest_element) > 0) {
    largest = right;
    largest_element = right_element;
  }

  if (largest != index) {
    void *largest_element = array_get_ref(heap->array, largest);
    void *index_element = array_get_ref(heap->array, index);

    array_set(heap->array, largest, index_element);
    array_set(heap->array, index, largest_element);
  }
}

static void heapify(struct heap *heap) {
  for (int i = array_size(heap->array) / 2 - 1; i >= 0; i--) {
    heapify_helper(heap, i);
  }
}

int heap_push(struct heap *heap, void *data) {
  array_append(heap->array, data);
  heapify(heap);

  return 0;
}

int heap_pop(struct heap *heap, void *data) {
  if (array_size(heap->array) == 0) {
    return -1;
  }

  void *first_element = array_get_ref(heap->array, 0);
  void *last_element = array_get_ref(heap->array, array_size(heap->array) - 1);
  array_set(heap->array, 0, last_element);
  array_set(heap->array, array_size(heap->array) - 1, first_element);

  array_pop(heap->array, data);
  heapify(heap);

  return 0;
}

void *heap_peek(struct heap *heap) {
  if (array_size(heap->array) == 0) {
    return NULL;
  }

  return array_get_ref(heap->array, 0);
}

int heap_empty(struct heap *heap) { return array_size(heap->array) == 0; }

void heap_destroy(struct heap *heap) {
  array_destroy(heap->array);
  free(heap);
}
