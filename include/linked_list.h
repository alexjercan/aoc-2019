#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct linked_list {
  void *data;
  struct linked_list *next;
};

int linked_list_append(struct linked_list **head, void *data);
void linked_list_free(struct linked_list *head);

#endif // LINKED_LIST_H
