#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct Queue_Elem{
  void *elem;
  struct Queue_Elem *next;
}Queue_Elem;

typedef struct Queue{
  Queue_Elem *first;
  Queue_Elem *last;
  int cnt;
}Queue;

Queue* createQueue();
int freeQueue(Queue* queue);

void* Dequeue(Queue* queue);
void Enqueue(Queue* queue, void *elem);

#endif