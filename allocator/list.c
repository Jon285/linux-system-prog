#include <stdlib.h>

#include "list.h"

void insert_front(void *data, MemoryList *list) {
  Node *node = malloc(sizeof(Node));

  node->data = data;
  node->next = NULL;
  node->previous = NULL;

  if (list->head == NULL) {
    list->head = node;
    return;
  }

  node->next = list->head;
  list->head->previous = node;
  list->head = node;
  return;
}
void insert_back(void *data, MemoryList *list) {}
void insert_at(void *data, unsigned int index, MemoryList *list) {}
Node *remove(void *data, MemoryList *index) {}
Node *remove_front(void *data, MemoryList *list) {}
Node *remove_back(void *data, MemoryList *list) {}
Node *remove_at(unsigned int index, MemoryList *list) {}
