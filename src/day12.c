#include "../include/day12.h"
#include "../include/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int planet_eq(struct planet p1, struct planet p2) {
  return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}

static int part2(struct array *array) {
    return 0;

  struct array *previous_states = array_new(1024, sizeof(struct array *));
  array_append(previous_states, &array);

  int index = 0;
  while (1) {
    struct array *current_state =
        array_new(array_size(array), sizeof(struct planet));
    array_copy(current_state, array);

    step_simulate(current_state);

    for (int i = 0; i < array_size(previous_states); i++) {
      struct array *previous_state =
          *(struct array **)array_get_ref(previous_states, i);

      int eq = 1;
      for (int j = 0; j < array_size(previous_state); j++) {
        struct planet p1 = *(struct planet *)array_get_ref(previous_state, j);
        struct planet p2 = *(struct planet *)array_get_ref(current_state, j);

        if (!planet_eq(p1, p2)) {
            eq = 0;
            break;
        }
      }
      if (eq) {
          return index - i;
      }
    }

    array_append(previous_states, &current_state);
    array = current_state;

    index++;
  }

  return 0;
}

void day12_solve(char *input, char *output) {
  struct array *array = array_new(4, sizeof(struct planet));
  parse_input(input, array);

  struct array *clone = array_new(array_size(array), sizeof(struct planet));
  array_copy(clone, array);

  sprintf(output, "Day12\nPart1: %d\nPart2: %d\n", part1(array), part2(clone));
}
