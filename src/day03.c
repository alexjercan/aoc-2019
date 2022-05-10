#include "../include/day03.h"
#include "../include/linked_list.h"

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

struct day03_data {
  struct linked_list *segments1;
  struct linked_list *segments2;
};

static struct linked_list *parse_line(char *line) {
  struct linked_list *segments = NULL;

  int x = 0;
  int y = 0;
  char *token = strtok(line, ",");
  while (token != NULL) {
    struct segment *segment = malloc(sizeof(struct segment));

    if (strncmp(token, "R", 1) == 0) {
      segment->start.x = x;
      segment->start.y = y;
      segment->end.x = x + atoi(token + 1);
      segment->end.y = y;
      x = segment->end.x;
    } else if (strncmp(token, "L", 1) == 0) {
      segment->start.x = x;
      segment->start.y = y;
      segment->end.x = x - atoi(token + 1);
      segment->end.y = y;
      x = segment->end.x;
    } else if (strncmp(token, "U", 1) == 0) {
      segment->start.x = x;
      segment->start.y = y;
      segment->end.x = x;
      segment->end.y = y + atoi(token + 1);
      y = segment->end.y;
    } else if (strncmp(token, "D", 1) == 0) {
      segment->start.x = x;
      segment->start.y = y;
      segment->end.x = x;
      segment->end.y = y - atoi(token + 1);
      y = segment->end.y;
    }

    linked_list_append(&segments, segment);
    token = strtok(NULL, ",");
  }

  return segments;
}

static struct day03_data parse_input(char *input) {
  struct day03_data data;

  char *line1 = strtok(input, "\n");
  char *line2 = strtok(NULL, "\n");

  data.segments1 = parse_line(line1);
  data.segments2 = parse_line(line2);

  return data;
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

static int part1(struct day03_data data) {
  struct linked_list *inters = NULL;

  for (struct linked_list *s1 = data.segments1; s1 != NULL; s1 = s1->next) {
    for (struct linked_list *s2 = data.segments2; s2 != NULL; s2 = s2->next) {
      struct point *point = segment_intersect((*(struct segment *)s1->data),
                                              *((struct segment *)s2->data));
      if (point != NULL) {
        linked_list_append(&inters, point);
      }
    }
  }

  int min_dist = -1;
  for (struct linked_list *i = inters; i != NULL; i = i->next) {
    struct point *point = i->data;
    int dist = abs(point->x) + abs(point->y);
    if (min_dist == -1 || dist < min_dist) {
      min_dist = dist;
    }
  }

  linked_list_free(inters);
  return min_dist;
}

static int part2(struct day03_data data) {
  struct linked_list *dists = NULL;

  struct point point1 = {0, 0};
  struct point point2 = {0, 0};

  int cost1 = 0;
  for (struct linked_list *s1 = data.segments1; s1 != NULL; s1 = s1->next) {
    int cost2 = 0;

    for (struct linked_list *s2 = data.segments2; s2 != NULL; s2 = s2->next) {
      struct point *point = segment_intersect((*(struct segment *)s1->data),
                                              *((struct segment *)s2->data));
      if (point != NULL) {
        int *dist = malloc(sizeof(int));
        *dist = cost1 + cost2;
        struct segment seg1 = {.start = point1, .end = *point};
        *dist += segment_length(seg1);
        struct segment seg2 = {.start = point2, .end = *point};
        *dist += segment_length(seg2);
        linked_list_append(&dists, dist);
      }

      free(point);
      cost2 += segment_length(*((struct segment *)s2->data));
      point2 = (*(struct segment *)s2->data).end;
    }

    cost1 += segment_length(*((struct segment *)s1->data));
    point1 = (*(struct segment *)s1->data).end;
  }

  int min_dist = -1;
  for (struct linked_list *i = dists; i != NULL; i = i->next) {
    int *dist = i->data;
    if (min_dist == -1 || *dist < min_dist) {
      min_dist = *dist;
    }
  }

  linked_list_free(dists);
  return min_dist;
}

void day03_solve(char *input, char *output) {
  struct day03_data data = parse_input(input);

  sprintf(output, "Day03\nPart1: %d\nPart2: %d\n", part1(data), part2(data));

  linked_list_free(data.segments1);
  linked_list_free(data.segments2);
}
