#include "../include/tuple.h"
#include "../include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t tuple_hash(const void *tuple) {
  const struct tuple *t = tuple;
  char buffer[100] = {0};
  sprintf(buffer, "%d,%d", t->x, t->y);
  return hash_sip(buffer, strlen(buffer), 0, 0);
}

int tuple_equals(const void *a, const void *b) {
  const struct tuple *t1 = a;
  const struct tuple *t2 = b;
  return t1->x == t2->x && t1->y == t2->y;
}
