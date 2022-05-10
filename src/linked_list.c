#include "../include/linked_list.h"

#include <stdlib.h>

int linked_list_append(struct linked_list **head, void *data) {
  struct linked_list *node = malloc(sizeof(struct linked_list));
  if (node == NULL) {
    return -1;
  }

  node->data = data;
  node->next = NULL;

  if (*head == NULL) {
    *head = node;
  } else {
    struct linked_list *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = node;
  }

  return 0;
}

void linked_list_free(struct linked_list *head) {
  struct linked_list *current = head;

  while (current != NULL) {
    struct linked_list *next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
}
