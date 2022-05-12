#ifndef UTIL_H
#define UTIL_H

#include "array.h"
#include "queue.h"

int array_get_int(struct array *array, int index);

int queue_dequeue_int(struct queue *queue);

#endif // UTIL_H