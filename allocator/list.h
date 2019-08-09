#ifndef __LIST_H_
#define __LIST_H_

// doubly linked list to keep track on the allocated memory

typedef struct Node {
  void *data;
  struct Node *next;
  struct Node *previous;
} Node;

typedef struct MemoryList {
  Node *head;
  Node *tail;
} MemoryList;

void insert_front(void *data, MemoryList *list);
void insert_back(void *data, MemoryList *list);
void insert_at(void *data, unsigned int index, MemoryList *list);
Node *remove(void *data, MemoryList *list);
Node *remove_front(void *data, MemoryList *list);
Node *remove_at(unsigned int index, MemoryList *list);

#endif // __LIST_H_
