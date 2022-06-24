#ifndef _GRAPH_H
#define _GRAPH_H

#include "Queue.h"

typedef struct GraphNode{
  int nextsCnt;
  void *elem;
  struct GraphNode** nexts;
}GraphNode;

void createConnectionMemory(GraphNode *graph, int tam);
void connectingGraph(GraphNode *graph, int numberOfConnections);
Queue* deepBreathTraverse(GraphNode *graph, int graphNodesCnt, int start);

#endif