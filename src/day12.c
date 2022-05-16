#include "../include/day12.h"
#include "../include/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long int gcd(long long int a, long long int b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

static long long int lcm(long long int a, long long int b) {
  return a * b / gcd(a, b);
}

struct planet {
  int x;
  int y;
  int z;

  int vx;
  int vy;
  int vz;
};

static void parse_point(char *line, struct planet *p) {
  char *x_str = strchr(line, 'x') + 2;
  char *y_str = strchr(line, 'y') + 2;
  char *z_str = strchr(line, 'z') + 2;

  p->x = atoi(x_str);
  p->y = atoi(y_str);
  p->z = atoi(z_str);

  p->vx = 0;
  p->vy = 0;
  p->vz = 0;
}

static void parse_input(char *input, struct array *array) {
  char *line = strtok(input, "\n");
  while (line != NULL) {
    struct planet p;
    parse_point(line, &p);
    array_append(array, &p);
    line = strtok(NULL, "\n");
  }
}

static void step_simulate(struct array *array) {
  for (int i = 0; i < array_size(array); i++) {
    for (int j = i + 1; j < array_size(array); j++) {
      struct planet *p1 = (struct planet *)array_get_ref(array, i);
      struct planet *p2 = (struct planet *)array_get_ref(array, j);

      if (p1->x < p2->x) {
        p1->vx++;
        p2->vx--;
      } else if (p1->x > p2->x) {
        p1->vx--;
        p2->vx++;
      }

      if (p1->y < p2->y) {
        p1->vy++;
        p2->vy--;
      } else if (p1->y > p2->y) {
        p1->vy--;
        p2->vy++;
      }

      if (p1->z < p2->z) {
        p1->vz++;
        p2->vz--;
      } else if (p1->z > p2->z) {
        p1->vz--;
        p2->vz++;
      }
    }
  }

  for (int i = 0; i < array_size(array); i++) {
    struct planet *p = (struct planet *)array_get_ref(array, i);

    p->x += p->vx;
    p->y += p->vy;
    p->z += p->vz;
  }
}

static int part1(struct array *array) {
  for (int i = 0; i < 1000; i++) {
    step_simulate(array);
  }

  int energy = 0;
  for (int i = 0; i < array_size(array); i++) {
    struct planet *p = (struct planet *)array_get_ref(array, i);
    energy += (abs(p->x) + abs(p->y) + abs(p->z)) *
              (abs(p->vx) + abs(p->vy) + abs(p->vz));
  }

  return energy;
}

static long long int part2(struct array *array) {
  struct array *prev_state =
      array_new(array_size(array), sizeof(struct planet));
  array_copy(prev_state, array);
  int found_x = -1, found_y = -1, found_z = -1;

  int index = 0;
  while (found_x == -1 || found_y == -1 || found_z == -1) {
    struct array *current_state =
        array_new(array_size(prev_state), sizeof(struct planet));
    array_copy(current_state, prev_state);

    step_simulate(current_state);

    int x_eq = 1, y_eq = 1, z_eq = 1;
    for (int j = 0; j < array_size(current_state); j++) {
      struct planet p1 = *(struct planet *)array_get_ref(array, j);
      struct planet p2 = *(struct planet *)array_get_ref(current_state, j);

      if (p1.x != p2.x) {
        x_eq = 0;
      }
      if (p1.y != p2.y) {
        y_eq = 0;
      }
      if (p1.z != p2.z) {
        z_eq = 0;
      }
    }

    if (x_eq && found_x == -1) {
      found_x = index + 2;
    }
    if (y_eq && found_y == -1) {
      found_y = index + 2;
    }
    if (z_eq && found_z == -1) {
      found_z = index + 2;
    }

    array_destroy(prev_state);
    prev_state = current_state;

    index++;
  }

  array_destroy(prev_state);

  return lcm(found_x, lcm(found_y, found_z));
}

void day12_solve(char *input, char *output) {
  struct array *array = array_new(4, sizeof(struct planet));
  parse_input(input, array);

  struct array *clone = array_new(array_size(array), sizeof(struct planet));
  array_copy(clone, array);

  sprintf(output, "Day12\nPart1: %d\nPart2: %lld\n", part1(array),
          part2(clone));

  array_destroy(array);
  array_destroy(clone);
}
