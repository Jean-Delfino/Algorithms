#include <stdlib.h>
#include "Queue.h"

int RemoveHead(Queue* queue);

Queue* createQueue(){
  return (Queue*) calloc(1 , sizeof(Queue));;
}

int freeQueue(Queue* queue){
  if(queue == NULL) return -1;
  
  while(RemoveHead(queue));

  free(queue);
  return 0;
}


void* Dequeue(Queue* queue){
  if(queue == NULL || queue->first == NULL) return NULL;

  Queue_Elem* waste = queue->first;
  void* res = waste->elem;
  
  queue->first = waste->next;
  free(waste);
  queue->cnt--;
  
  if(!queue->cnt){
    queue->last = NULL;
  }
  
  return res;
}

void Enqueue(Queue* queue, void *elem){
  if(queue == NULL) return;

  Queue_Elem *newElem = malloc(1 * sizeof(Queue_Elem));

  newElem->elem = elem;
  newElem->next = NULL;

  if(queue->first == NULL){
    queue->first = newElem;
  }else{
    queue->last->next = newElem;
  }

  queue->last = newElem;
  queue->cnt++;
}

int RemoveHead(Queue* queue){
  if(queue->first == NULL) return 0;

  Queue_Elem* waste = queue->first;
  queue->first = queue->first->next;
  queue->cnt--;

  free(waste);
  
  return 1;
}