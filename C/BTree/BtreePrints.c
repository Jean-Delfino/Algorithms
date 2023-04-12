#include "BtreePrints.h"


void printPage(node *buffer) {
	int i;

	for (i = 0; i < buffer->cnt; i++) {
		printf("Key %d = %d\n", i, buffer->keys[i]);
	}

	printf("Count = %d\n", buffer->cnt);
	printf("Buffer leaf = %d\n", buffer->leaf);

	printf("\n");

  	for (i = 0; i < buffer->cnt; i++) {
		printf("Offset %d = %lu\n",i, buffer->offsets[i]);
	}
  
  printf("\n");

	for (i = 0; i < buffer->cnt + 1; i++) {
		printf("Children %d = %d\n",i, buffer->children[i]);
	}
}

void printTree(FILE *arq, node *buffer, long qtdNode) {
	int i;

	for (i = 0; i < qtdNode; i++) {
		printf("PAGE = %d\n\n", i);
		read(buffer, i, arq);
		printPage(buffer);
		printf("\n");
	}
}

void printNodeBreak(node_break *test){
  printf("Key = %d\n\n", test->key);
  
  printf("Left Child = %d\n", test->leftChildren);
  printf("Right Child = %d\n\n",  test->rightChildren);
}