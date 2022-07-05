#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/day01.h"
#include "./include/day02.h"
#include "./include/day03.h"
#include "./include/day04.h"
#include "./include/day05.h"
#include "./include/day06.h"
#include "./include/day07.h"
#include "./include/day08.h"
#include "./include/day09.h"
#include "./include/day10.h"
#include "./include/day11.h"
#include "./include/day12.h"
#include "./include/day13.h"
#include "./include/day14.h"
#include "./include/day15.h"
#include "./include/day16.h"
#include "./include/day17.h"
#include "./include/day18.h"
#include "./include/day19.h"
#include "./include/day20.h"
#include "./include/day21.h"
#include "./include/day22.h"
#include "./include/day23.h"
#include "./include/day24.h"
#include "./include/day25.h"

#define BUF_SIZE 4096

int read_input(char **content) {
  int size = 0;
  *content = calloc(sizeof(char), BUF_SIZE + 1);
  if (*content == NULL) {
    return -1;
  }

  char buffer[BUF_SIZE];
  while (fgets(buffer, BUF_SIZE, stdin) > 0) {
    size += strlen(buffer);
    char *tmp = realloc(*content, size + 1);

    if (tmp == NULL) {
      free(*content);
      return -1;
    }

    *content = tmp;
    strcat(*content, buffer);
  }

  return size;
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage: ./main <day_number>\n");
    return 1;
  }

  int day = atoi(argv[1]);

  char *content = NULL;
  int size = read_input(&content);

  if (size < 0) {
    printf("Failed to read input\n");
    return 1;
  }

  char output[BUF_SIZE];
  memset(output, 0, BUF_SIZE);
  switch (day) {
  case 1:
    day01_solve(content, output);
    break;
  case 2:
    day02_solve(content, output);
    break;
  case 3:
    day03_solve(content, output);
    break;
  case 4:
    day04_solve(content, output);
    break;
  case 5:
    day05_solve(content, output);
    break;
  case 6:
    day06_solve(content, output);
    break;
  case 7:
    day07_solve(content, output);
    break;
  case 8:
    day08_solve(content, output);
    break;
  case 9:
    day09_solve(content, output);
    break;
  case 10:
    day10_solve(content, output);
    break;
  case 11:
    day11_solve(content, output);
    break;
  case 12:
    day12_solve(content, output);
    break;
  case 13:
    day13_solve(content, output);
    break;
  case 14:
    day14_solve(content, output);
    break;
  case 15:
    day15_solve(content, output);
    break;
  case 16:
    day16_solve(content, output);
    break;
  case 17:
    day17_solve(content, output);
    break;
  case 18:
    day18_solve(content, output);
    break;
  case 19:
    day19_solve(content, output);
    break;
  case 20:
    day20_solve(content, output);
    break;
  case 21:
    day21_solve(content, output);
    break;
  case 22:
    day22_solve(content, output);
    break;
  case 23:
    day23_solve(content, output);
    break;
  case 24:
    day24_solve(content, output);
    break;
  case 25:
    day25_solve(content, output);
    break;
  default:
    printf("Day %d not found\n", day);
  }
  printf("%s\n", output);

  free(content);
  return 0;
}
