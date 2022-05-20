#include "../include/day16.h"
#include "../include/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 1024

static int pattern[4] = {0, 1, 0, -1};

static void parse_input(char *input, struct array *array) {
  char *trimmed = strtok(input, "\n");

  for (int i = 0; i < strlen(trimmed); i++) {
    int value = input[i] - '0';
    array_append(array, &value);
  }
}

static int compute_digit(struct array *array, int index) {
  int result = 0;

  for (int i = 0; i < array_size(array); i++) {
    int value = *(int *)array_get_ref(array, i);
    int pattern_index = (i + 1) / (index + 1);
    int pattern_value = pattern[(pattern_index) % 4];
    result += value * pattern_value;
  }

  return abs(result) % 10;
}

static void apply_phase(struct array *array) {
  struct array *new_array = array_new(CAPACITY, sizeof(int));

  for (int i = 0; i < array_size(array); i++) {
    int value = compute_digit(array, i);
    array_append(new_array, &value);
  }

  array_copy(array, new_array);
  array_destroy(new_array);
}

static char *part1(struct array *array) {
  for (int i = 0; i < 100; i++) {
    apply_phase(array);
  }

  char *result = malloc(sizeof(char) * 9);
  for (int i = 0; i < 8; i++) {
    result[i] = *(int *)array_get_ref(array, i) + '0';
  }
  result[8] = '\0';

  return result;
}

static char *part2(struct array *array) {
  char *start_str = malloc(sizeof(char) * 8);
  for (int i = 0; i < 7; i++) {
    start_str[i] = *(int *)array_get_ref(array, i) + '0';
  }
  start_str[7] = '\0';

  int start = atol(start_str);
  int end = array_size(array) * 10000;
  int capacity = end - start;

  free(start_str);

  struct array *new_array = array_new(capacity, sizeof(int));
  for (int i = start; i < end; i++) {
    int value = *(int *)array_get_ref(array, i % array_size(array));
    array_append(new_array, &value);
  }

  for (int i = 0; i < 100; i++) {
    int total = 0;
    struct array *sums = array_new(capacity + 1, sizeof(int));
    array_append(sums, &total);

    for (int j = 0; j < array_size(new_array); j++) {
      total += *(int *)array_get_ref(new_array, j);
      array_append(sums, &total);
    }

    int last_value = *(int *)array_get_ref(sums, array_size(sums) - 1);
    for (int j = 0; j < array_size(new_array); j++) {
      int new_value = (last_value - *(int *)array_get_ref(sums, j)) % 10;
      array_set(new_array, j, &new_value);
    }

    array_destroy(sums);
  }

  char *result = malloc(sizeof(char) * 9);
  for (int i = 0; i < 8; i++) {
    result[i] = *(int *)array_get_ref(new_array, i) + '0';
  }
  result[8] = '\0';

  array_destroy(new_array);

  return result;
}

void day16_solve(char *input, char *output) {
  struct array *array = array_new(CAPACITY, sizeof(int));
  parse_input(input, array);

  struct array *clone = array_new(CAPACITY, sizeof(int));
  array_copy(clone, array);

  char *result1 = part1(array);
  char *result2 = part2(clone);
  sprintf(output, "Day16\nPart1: %s\nPart2: %s\n", result1, result2);

  free(result1);
  free(result2);
  array_destroy(array);
  array_destroy(clone);
}
