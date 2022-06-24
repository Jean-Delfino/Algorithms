#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

void createConnectionMemory(GraphNode *graph, int tam){
  int i;

  for(i = 0; i < tam; i++){
    graph[i].nexts = (GraphNode **) malloc(1 * sizeof(GraphNode*));
  }
}

void connectingGraph(GraphNode *graph, int numberOfConnections){
  int origin, destination;
  int i;

  for(i = 0; i < numberOfConnections; i++){
    scanf("%d %d" ,&origin, &destination);

    graph[origin].nexts = realloc(graph[origin].nexts, 
      (graph[origin].nextsCnt + 1) * sizeof(GraphNode*));
    
    graph[origin].nexts[graph[origin].nextsCnt] = 
      &graph[destination];
    graph[origin].nextsCnt++;
  }
}

void deepBreathTraverseRecursion(GraphNode *node, Queue* queue){
  if(node == NULL) return;
  
  int i;
  
  Enqueue(queue, (node)->elem);

  for(i = 0; i < (node)->nextsCnt; i++){
    deepBreathTraverseRecursion((node->nexts[i]), queue);
  }
}

Queue* deepBreathTraverse(GraphNode *graph,int graphNodesCnt, int start){
  Queue* queue = createQueue();
  
  deepBreathTraverseRecursion(&graph[start], queue);

  return queue;
}