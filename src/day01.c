#include "../include/day01.h"
#include "../include/linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct linked_list *parse_input(char *input) {
  struct linked_list *head = NULL;

  char *line = strtok(input, "\n");
  while (line != NULL) {
    int value = atoi(line);
    int *data = malloc(sizeof(int));
    *data = value;
    linked_list_append(&head, (void *)data);
    line = strtok(NULL, "\n");
  }

  return head;
}

static int part1(struct linked_list *head) {
  int sum = 0;

  struct linked_list *current = head;
  while (current != NULL) {
    sum += *((int *)current->data) / 3 - 2;
    current = current->next;
  }

  return sum;
}

static int part2(struct linked_list *head) {
  int sum = 0;

  struct linked_list *current = head;
  while (current != NULL) {
    int fuel = *((int *)current->data) / 3 - 2;
    while (fuel > 0) {
      sum += fuel;
      fuel = fuel / 3 - 2;
    }

    current = current->next;
  }

  return sum;
}

void day01_solve(char *input, char *output) {
  struct linked_list *head = parse_input(input);

  sprintf(output, "Day01\nPart1: %d\nPart2: %d\n", part1(head), part2(head));

  linked_list_free(head);
}
