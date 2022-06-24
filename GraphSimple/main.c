#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

typedef struct{
  int ID;
  char* nameMission;
  char *descriptionMission;
}DungeonMission;

char* stringRead(int *end);

DungeonMission* readMission(int IDmission, int *end){
  DungeonMission *newDM = 
    malloc(1 * sizeof(DungeonMission));

  newDM->ID = IDmission;
  newDM->nameMission = stringRead(end);
  newDM->descriptionMission = stringRead(end);
  
  return newDM;
}

void printMission(DungeonMission *dm){
  if(dm == NULL) return;

  printf("Quest{\n\tID= '%d'\n", dm->ID);
  printf("\tname= %s\n", dm->nameMission);
  printf("\tdescription= %s\n", dm->descriptionMission);
  printf("}\n");
}


void printGraph(GraphNode *graph, int tam){
  int i, j;

  for(i = 0; i < tam; i++){
    printMission((DungeonMission*) graph[i].elem);
    printf("\n\n");
    printf("NODE CNT %d = %d\n" , i, graph[i].nextsCnt);
    for(j = 0; j < graph[i].nextsCnt; j++){
      printMission((DungeonMission*) graph[i].nexts[j]->elem);
    }
    printf("-------\n\n");
  }
}


char* stringRead(int *end){
  char c;
  char *stringRet;
  int size = -1;

  stringRet = malloc(1 * sizeof(char));
  
  do{
    size++;
    stringRet = realloc(stringRet, (size+1) * sizeof(char));
    
    c = getc(stdin);
    stringRet[size] = c;
    
    if(c == EOF){
      *end = 1;
      break;
    }
  }while(c != '\n');

  stringRet[size] = '\0'; //Overwrites the separator

  return stringRet;
}

void printMissionsPath(GraphNode *graph, int nodesCnt){
  int startPoint, i, j;
  scanf("%d", &startPoint);
  
  Queue* queue = deepBreathTraverse(graph, nodesCnt, startPoint);
  
  int printCnt = queue->cnt;

  DungeonMission *vectorMission = malloc(printCnt * sizeof(DungeonMission));
  DungeonMission aux;
  
  for(i = 0; i < printCnt; i++){
    //Ok because i don't want to change the memory assign in any way
    vectorMission[i] = *((DungeonMission*) Dequeue(queue));
  } 

  //Yes, bubble sort
  for(i = 0; i < printCnt; i++){
    for(j = 0; j < printCnt - 1; j++){
      if(vectorMission[j].ID > vectorMission[j+1].ID){
        aux = vectorMission[j+1];
        vectorMission[j+1] = vectorMission[j];
        vectorMission[j] = aux;
      }
    }
  }
  
  for(i = 0; i < printCnt; i++){
    printMission(&vectorMission[i]);
  }
}

int main(void) {
  int i , numberOfNodes, numberOfConnections;
  int end; // Not needed, but good practice

  scanf("%d", &numberOfNodes);
  fflush(stdin);
  char enterGarbage = getc(stdin);
  
  GraphNode *graph = 
    calloc(numberOfNodes, sizeof(GraphNode));
  
  for(i = 0 ; i < numberOfNodes; i++){
    graph[i].elem = readMission(i, &end);
  }

  createConnectionMemory(graph, numberOfNodes);

  scanf("%d" , &numberOfConnections);
    
  connectingGraph(graph, numberOfConnections);

  //printGraph(graph, numberOfNodes);
  
  printMissionsPath(graph, numberOfNodes);
  
  return 0;
}
/*
4
aa
bb
cc
dd
ee
ff
gg
hh
4
0 1
1 2
1 3
3 2
0
*/

/*
#define maxDrop 10
#define maxDropValue "X"
#define spare "\"
#define noMoreDrop "_"
#define maxQtdPlay 2

int main(){
  int qtdRounds = 10;
  int i;
  int score, int accumulation, int finalScore;
  Lista print;

  for(i = 0; i < qtdRound-1 ; i++){
    score = play(maxQtdPlay, maxDrop, print);
    accumulation = accumulationCalculation(score, accumulation);
    finalScore += scoreCalculation(score, accumulation);
    print.Add("|");
  }

  if(accumulation != 0){
    for(i = 0; i < maxQtdPlay ; i++){
      score = play(1, maxDrop, print);
      accumulation = accumulationCalculation(score, accumulation);
      finalScore += scoreCalculation(score, accumulation);
    }
  }
}

  for(i = 0; i < qtdRound-1 ; i++){
    score = play(maxQtdPlay, maxDrop, print);
    accumulation = accumulationCalculation(score, accumulation);
    finalScore += scoreCalculation(score, accumulation);
    print.Add("|");
  }

int round(int qtdRound, int accumulation){
  if(!qtdRound) return 0;

  score = play(2, 10, print);
  accumulation = accumulationCalculation(score, accumulation);
  finalScore = scoreCalculation(score, accumulation) + round(qtdRound-1, accumulation);
  print.Add("|");
  return finalScore;
}

int pinDrop(int score, int remainingPins){
  return remainingPin-score;
}

int accumulationCalculation(int score, int accumulation){
  if(score > maxDrop){
    return accumulation + 10;
  }

  return 0;
}

int scoreCalculation(int score, int accumulation){
  return score + accumulation;
}

int play(int qtdPlay, int remainingPins, List point){
  if(!qtdPlay) return maxDrop - remainingPin;

  char* value = malloc(2* sizeof(char));
  int score;

  if(remaininPin){
    value = noMoreDrop;
    ballsDrop = 0;
  }else{
    scanf("%d", score);

    if(score == maxDrop){
      value = maxDropValue;
      if(qtdPlay == maxQtdPlay){
        score++; //This makes remainingPins negative
        //Which makes the final return be 11
      }
    }else if(score == remainingPins){
      value = spare;
    }
    else{
      value = intToString(score);
    }
  }

  point.Add(value); 
  play(qtdPlay-1, pinDrop(score, remainingPins), List point);
}
#define maxDrop 10
#define maxDropValue "X"
#define spare "\"
#define noMoreDrop "_"
#define maxQtdPlay 2
*/