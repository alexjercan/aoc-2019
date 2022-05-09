#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/day01.h"

struct Node {
    void *data;
    struct Node *next;
};

int linked_list_append(struct Node **head, void *data) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return -1;
    }

    node->data = data;
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    } else {
        struct Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    return 0;
}

struct Node *parse_input(char *input) {
    struct Node *head = NULL;

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

int part1(struct Node *head) {
    int sum = 0;

    struct Node *current = head;
    while (current != NULL) {
        sum += *((int *)current->data) / 3 - 2;
        current = current->next;
    }

    return sum;
}

int part2(struct Node *head) {
    int sum = 0;

    struct Node *current = head;
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
    struct Node *head = parse_input(input);

    sprintf(output, "Day01\nPart1: %d\nPart2: %d\n", part1(head), part2(head));
}
