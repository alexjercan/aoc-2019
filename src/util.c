#include "../include/util.h"

int array_get_int(struct array *array, int index) {
  int value;
  array_get(array, index, &value);
  return value;
}

int array_append_int(struct array *array, int value) {
  return array_append(array, &value);
}

int queue_dequeue_int(struct queue *queue) {
  int value;
  queue_dequeue(queue, &value);
  return value;
}
