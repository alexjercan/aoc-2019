#ifndef TUPLE_H
#define TUPLE_H

#include <stdlib.h>

struct tuple {
  int x;
  int y;
};

size_t tuple_hash(const void *tuple);
int tuple_equals(const void *a, const void *b);

#endif // TUPLE_H
