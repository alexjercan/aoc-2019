#include "../include/day04.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct day04_data {
  int lowest;
  int highest;
};

struct day04_data parse_input(char *input) {
  struct day04_data data;

  char *lowest = strtok(input, "-");
  char *highest = strtok(NULL, "\n");

  data.lowest = atoi(lowest);
  data.highest = atoi(highest);

  return data;
}

int is_valid_password(int password) {
  int ok = 0;
  int last_digit = 10;
  while (password > 0) {
    int digit = password % 10;
    if (digit > last_digit) {
      return 0;
    }
    if (digit == last_digit) {
      ok = 1;
    }

    last_digit = digit;
    password /= 10;
  }

  return ok;
}

int is_valid_password_2(int password) {
  int ok = 0;
  int last_last_last_digit = 12;
  int last_last_digit = 11;
  int last_digit = 10;
  while (password > 0) {
    int digit = password % 10;
    if (digit > last_digit) {
      return 0;
    }
    if (last_last_last_digit != last_digit && last_last_digit == last_digit &&
        digit != last_digit) {
      ok = 1;
    }

    last_last_last_digit = last_last_digit;
    last_last_digit = last_digit;
    last_digit = digit;
    password /= 10;
  }
  if (last_last_last_digit != last_digit && last_last_digit == last_digit) {
    ok = 1;
  }

  return ok;
}

int part1(struct day04_data data) {
  int count = 0;

  for (int p = data.lowest; p <= data.highest; p++) {
    if (is_valid_password(p)) {
      count++;
    }
  }

  return count;
}

int part2(struct day04_data data) {
  int count = 0;

  for (int p = data.lowest; p <= data.highest; p++) {
    if (is_valid_password_2(p)) {
      count++;
    }
  }

  return count;
}

void day04_solve(char *input, char *output) {
  struct day04_data data = parse_input(input);

  sprintf(output, "Day04\nPart1: %d\nPart2: %d\n", part1(data), part2(data));
}
