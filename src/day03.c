#include "../include/day03.h"
#include "../include/array.h"
#include "../include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
  int x;
  int y;
};

struct segment {
  struct point start;
  struct point end;
};

static void parse_line(char *line, struct array *array) {
  int x = 0;
  int y = 0;
  char *token = strtok(line, ",");
  while (token != NULL) {
    struct segment segment;

    if (strncmp(token, "R", 1) == 0) {
      segment.start.x = x;
      segment.start.y = y;
      segment.end.x = x + atoi(token + 1);
      segment.end.y = y;
      x = segment.end.x;
    } else if (strncmp(token, "L", 1) == 0) {
      segment.start.x = x;
      segment.start.y = y;
      segment.end.x = x - atoi(token + 1);
      segment.end.y = y;
      x = segment.end.x;
    } else if (strncmp(token, "U", 1) == 0) {
      segment.start.x = x;
      segment.start.y = y;
      segment.end.x = x;
      segment.end.y = y + atoi(token + 1);
      y = segment.end.y;
    } else if (strncmp(token, "D", 1) == 0) {
      segment.start.x = x;
      segment.start.y = y;
      segment.end.x = x;
      segment.end.y = y - atoi(token + 1);
      y = segment.end.y;
    }

    array_append(array, &segment);
    token = strtok(NULL, ",");
  }
}

static void parse_input(char *input, struct array *segments1,
                        struct array *segments2) {
  char *line1 = strtok(input, "\n");
  char *line2 = strtok(NULL, "\n");

  parse_line(line1, segments1);
  parse_line(line2, segments2);
}

static struct point *segment_intersect(struct segment segment1,
                                       struct segment segment2) {
  struct point *point = NULL;

  int s10_x = segment1.end.x - segment1.start.x;
  int s10_y = segment1.end.y - segment1.start.y;
  int s32_x = segment2.end.x - segment2.start.x;
  int s32_y = segment2.end.y - segment2.start.y;

  int denom = s10_x * s32_y - s32_x * s10_y;

  if (denom == 0) {
    return NULL;
  }

  int s02_x = segment1.start.x - segment2.start.x;
  int s02_y = segment1.start.y - segment2.start.y;

  int s_numer = s10_x * s02_y - s02_x * s10_y;

  if ((s_numer < 0) == (denom > 0)) {
    return NULL;
  }

  int t_numer = s32_x * s02_y - s02_x * s32_y;

  if ((t_numer < 0) == (denom > 0)) {
    return NULL;
  }

  if ((s_numer > denom) == (denom > 0) || (t_numer > denom) == (denom > 0)) {
    return NULL;
  }

  float t = t_numer / (float)denom;

  point = malloc(sizeof(struct point));
  point->x = segment1.start.x + (t * s10_x);
  point->y = segment1.start.y + (t * s10_y);

  return point;
}

static int segment_length(struct segment segment) {
  return abs(segment.end.x - segment.start.x) +
         abs(segment.end.y - segment.start.y);
}

static int part1(struct array *segments1, struct array *segments2) {
  struct array *inters = array_new(1024, sizeof(struct point *));

  for (int i = 0; i < array_size(segments1); i++) {
    for (int j = 0; j < array_size(segments2); j++) {
      struct segment segment1;
      array_get(segments1, i, &segment1);
      struct segment segment2;
      array_get(segments2, j, &segment2);

      struct point *point = segment_intersect(segment1, segment2);

      if (point != NULL) {
        array_append(inters, &point);
      }
    }
  }

  int min_dist = -1;
  for (int i = 0; i < array_size(inters); i++) {
    struct point *point;
    array_get(inters, i, &point);
    int dist = abs(point->x) + abs(point->y);
    if (min_dist == -1 || dist < min_dist) {
      min_dist = dist;
    }
  }

  for (int i = 0; i < array_size(inters); i++) {
    struct point *point = NULL;
    array_get(inters, i, &point);
    free(point);
  }
  array_destroy(inters);
  return min_dist;
}

static int part2(struct array *segments1, struct array *segments2) {
  struct array *dists = array_new(1024, sizeof(int));

  struct point point1 = {0, 0};
  struct point point2 = {0, 0};

  int cost1 = 0;
  for (int i = 0; i < array_size(segments1); i++) {
    struct segment segment1;
    array_get(segments1, i, &segment1);

    int cost2 = 0;
    for (int j = 0; j < array_size(segments2); j++) {
      struct segment segment2;
      array_get(segments2, j, &segment2);

      struct point *point = segment_intersect(segment1, segment2);

      if (point != NULL) {
        struct segment seg1 = {.start = point1, .end = *point};
        struct segment seg2 = {.start = point2, .end = *point};
        int dist = cost1 + cost2 + segment_length(seg1) + segment_length(seg2);
        array_append(dists, &dist);
        free(point);
      }

      cost2 += segment_length(segment2);
      point2 = segment2.end;
    }

    cost1 += segment_length(segment1);
    point1 = segment1.end;
  }

  int min_dist = -1;
  for (int i = 0; i < array_size(dists); i++) {
    int dist = array_get_int(dists, i);
    if (min_dist == -1 || dist < min_dist) {
      min_dist = dist;
    }
  }

  array_destroy(dists);
  return min_dist;
}

void day03_solve(char *input, char *output) {
  struct array *segments1 = array_new(1024, sizeof(struct segment));
  struct array *segments2 = array_new(1024, sizeof(struct segment));
  parse_input(input, segments1, segments2);

  sprintf(output, "Day03\nPart1: %d\nPart2: %d\n", part1(segments1, segments2),
          part2(segments1, segments2));

  array_destroy(segments1);
  array_destroy(segments2);
}
