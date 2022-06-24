#ifndef _BTREE_PRINTS_H
#define _BTREE_PRINTS_H

#include "Btree.h"

void printPage(node *buffer);
void printTree(FILE *arq, node *buffer, long qtdNode);
void printNodeBreak(node_break *test);

#endif


