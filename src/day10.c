#include "../include/day10.h"
#include "../include/array.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int sign(int x) { return (x > 0) - (x < 0); }

struct point {
  int x;
  int y;
};

static void parse_input(char *input, struct array *points) {
  char *line = strtok(input, "\n");

  for (int i = 0; line != NULL; i++, line = strtok(NULL, "\n")) {
    for (int j = 0; j < strlen(line); j++) {
      if (line[j] != '.') {
        struct point p = {j, i};
        array_append(points, &p);
      }
    }
  }
}

static int part1(struct array *points) {
  int max_count = 0;

  for (int i = 0; i < array_size(points); i++) {
    struct point proposed_center = *(struct point *)array_get_ref(points, i);

    int count = 0;
    for (int j = 0; j < array_size(points); j++) {
      if (i == j) {
        continue;
      }

      struct point p = *(struct point *)array_get_ref(points, j);
      int dx_p = p.x - proposed_center.x;
      int dy_p = p.y - proposed_center.y;
      int d_p = dx_p * dx_p + dy_p * dy_p;

      int is_visible = 1;
      for (int k = 0; k < array_size(points); k++) {
        if (k == i || k == j) {
          continue;
        }

        struct point q = *(struct point *)array_get_ref(points, k);
        int dx_q = q.x - proposed_center.x;
        int dy_q = q.y - proposed_center.y;
        int d_q = dx_q * dx_q + dy_q * dy_q;

        if (dx_q * dy_p == dx_p * dy_q && d_q < d_p &&
            sign(dx_q) == sign(dx_p) && sign(dy_q) == sign(dy_p)) {
          is_visible = 0;
          break;
        }
      }

      if (is_visible) {
        count++;
      }
    }

    if (count > max_count) {
      max_count = count;
    }
  }

  return max_count;
}

static float angle(struct point p, struct point q) {
  int dot = p.x * q.x + p.y * q.y;
  int det = p.x * q.y - p.y * q.x;

  float angle = atan2(det, dot);
  if (angle < 0) {
    angle += 2 * M_PI;
  }

  return angle;
}

static int part2(struct array *points) {
  int max_count = 0;
  struct point max_center = {0, 0};

  for (int i = 0; i < array_size(points); i++) {
    struct point proposed_center = *(struct point *)array_get_ref(points, i);

    int count = 0;
    for (int j = 0; j < array_size(points); j++) {
      if (i == j) {
        continue;
      }

      struct point p = *(struct point *)array_get_ref(points, j);
      int dx_p = p.x - proposed_center.x;
      int dy_p = p.y - proposed_center.y;
      int d_p = dx_p * dx_p + dy_p * dy_p;

      int is_visible = 1;
      for (int k = 0; k < array_size(points); k++) {
        if (k == i || k == j) {
          continue;
        }

        struct point q = *(struct point *)array_get_ref(points, k);
        int dx_q = q.x - proposed_center.x;
        int dy_q = q.y - proposed_center.y;
        int d_q = dx_q * dx_q + dy_q * dy_q;

        if (dx_q * dy_p == dx_p * dy_q && d_q < d_p &&
            sign(dx_q) == sign(dx_p) && sign(dy_q) == sign(dy_p)) {
          is_visible = 0;
          break;
        }
      }

      if (is_visible) {
        count++;
      }
    }

    if (count > max_count) {
      max_count = count;
      max_center = proposed_center;
    }
  }

  struct array *sorted_points = array_new(1024, sizeof(struct point));
  for (int i = 0; i < array_size(points); i++) {
    struct point p = *(struct point *)array_get_ref(points, i);
    if (p.x == max_center.x && p.y == max_center.y) {
      continue;
    }

    array_append(sorted_points, &p);
  }

  struct point unit_j = {0, -1};
  for (int i = 0; i < array_size(sorted_points); i++) {
    for (int j = i + 1; j < array_size(sorted_points); j++) {
      struct point p = *(struct point *)array_get_ref(sorted_points, i);
      struct point q = *(struct point *)array_get_ref(sorted_points, j);

      int dx_p = p.x - max_center.x;
      int dy_p = p.y - max_center.y;
      int d_p = dx_p * dx_p + dy_p * dy_p;
      struct point dp = {dx_p, dy_p};
      float angle_p = angle(unit_j, dp);

      int dx_q = q.x - max_center.x;
      int dy_q = q.y - max_center.y;
      int d_q = dx_q * dx_q + dy_q * dy_q;
      struct point dq = {dx_q, dy_q};
      float angle_q = angle(unit_j, dq);

      if (angle_q < angle_p || (angle_q == angle_p && d_q < d_p)) {
        array_set(sorted_points, i, &q);
        array_set(sorted_points, j, &p);
      }
    }
  }

  int count = 0;
  while (1) {
    float last_angle = -1.0f;

    struct array *skipped_points = array_new(1024, sizeof(struct point));
    for (int i = 0; i < array_size(sorted_points); i++) {
      struct point p = *(struct point *)array_get_ref(sorted_points, i);
      int dx_p = p.x - max_center.x;
      int dy_p = p.y - max_center.y;
      struct point dp = {dx_p, dy_p};
      float angle_p = angle(unit_j, dp);

      if (angle_p != last_angle) {
        count++;
      } else {
        array_append(skipped_points, &p);
      }

      if (count == 200) {
        array_destroy(skipped_points);
        array_destroy(sorted_points);
        return p.x * 100 + p.y;
      }

      last_angle = angle_p;
    }

    array_destroy(sorted_points);
    sorted_points = skipped_points;
  }
}

void day10_solve(char *input, char *output) {
  struct array *points = array_new(1024, sizeof(struct point));
  parse_input(input, points);

  sprintf(output, "Day10\nPart1: %d\nPart2: %d\n", part1(points),
          part2(points));

  array_destroy(points);
}
