#ifndef _BTREE_H
#define _BTREE_H
// There are some specific rules i seted to make it more fun to develop

// 1- Root will always be page 0
// 2- There will be no list, queue etc To the insert, just recursion
//#define LIMIT 4
#define LIMIT 240
#define PAGE 4096
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int key;
	long offset;
	int leftChildren;
	int rightChildren;
} node_break;

typedef struct node{
	int cnt;	// counter of keys
	int keys[LIMIT - 1];	 // keys in the node
	long offsets[LIMIT - 1]; // offset of the keys in memory
	int children[LIMIT]; // node of the children  - indicates which "page" he is
	char leaf; //0 for node, 1 for leaf, 2 for root
} node;

node *createTreeBuffer();
long checkIfTreeReturnQtdPages(node* buffer, FILE *arq);

void read(node *buffer, int localization, FILE *arq);
void readRoot(node *buffer, FILE *arq);
void writeRoot(node *buffer, FILE *arq);
int searchPosition(node *buffer, int key);
int insertKey(node *buffer, int key, long offset, FILE *arq, long *cntPages);
//Passing a key return offset if the key exists
long searchOffsetInTree(node *buffer, FILE *arq, int key, int *find);

#endif

