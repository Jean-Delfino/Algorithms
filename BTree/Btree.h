//There are some specific rules i seted to make it more fun to develop

//1- Root will always be page 0
//2- There will be no list, queue etc To the insert, just recursion

#define LIMIT 4
//#define ordem 240
#define page 4096
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
  int key;
  long offset;
  int leftChildren;
  int rightChildren;
}node_break;

typedef struct{
  int cnt; //counter of keys
  int keys[LIMIT-1]; //keys in the node 
  long offsets[LIMIT-1]; // offset of the keys in memory
  int children[LIMIT]; // node of the children  - indicates which "page" he is
  char leaf;
}node; 

int insertKey(node* buffer, int key , long offset , FILE *arq , long *cntPages);
