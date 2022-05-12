#include "../include/day04.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_input(char *input, int *lowest, int *highest) {
  char *lowest_str = strtok(input, "-");
  char *highest_str = strtok(NULL, "\n");

  *lowest = atoi(lowest_str);
  *highest = atoi(highest_str);
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

int part1(int lowest, int highest) {
  int count = 0;

  for (int p = lowest; p <= highest; p++) {
    if (is_valid_password(p)) {
      count++;
    }
  }

  return count;
}

int part2(int lowest, int highest) {
  int count = 0;

  for (int p = lowest; p <= highest; p++) {
    if (is_valid_password_2(p)) {
      count++;
    }
  }

  return count;
}

void day04_solve(char *input, char *output) {
  int lowest, highest;
  parse_input(input, &lowest, &highest);

  sprintf(output, "Day04\nPart1: %d\nPart2: %d\n", part1(lowest, highest),
          part2(lowest, highest));
}
