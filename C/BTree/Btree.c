#include "Btree.h"
#include "BtreePrints.h"

node_break *insertKeyNode(int treeNode, node *buffer,
	node_break *key,FILE *arq, long *cntPages);

node_break *insertKeyLeaf(int treeNode,node *buffer, 
    int key,long offset,FILE *arq,long *cntPages);

//Tree creation basically
node* createTreeBuffer(){
  node *buff = malloc(1 * sizeof(node));
  buff->leaf = 1;
  buff->cnt = 0;
  return buff;
}

long getQtdTreeNode(FILE *arq){
  fseek(arq, 0, SEEK_END);
  long qtd = ftell(arq);

  if(qtd == 0) return -1;

  return (qtd/PAGE);
}

void read(node *buffer, int localization, FILE *arq) {
	fseek(arq, localization * PAGE, SEEK_SET);

	fread(buffer, sizeof(node), 1, arq);
}

void readRoot(node *buffer, FILE *arq){
  read(buffer, 0, arq);
}

void write(node *buffer, int localization, FILE *arq){
	fseek(arq, localization * PAGE, SEEK_SET);

	fwrite(buffer, sizeof(node), 1, arq);
  fflush(arq);
}

long checkIfTreeReturnQtdPages(node *buffer , FILE *arq){
  long qtdNode = getQtdTreeNode(arq);
  
	if (qtdNode < 0) {
		writeRoot(buffer, arq);
    qtdNode = 1;
	} else {
		qtdNode++;
	}

  return qtdNode;
}

void writeRoot(node *buffer, FILE *arq){
  write(buffer, 0, arq);
}

/*
  For some reason you can't have pushVector only
  Like a generic *void being passed and the bytesize also be passed
  I tried at least, and it simply don't generates right anwsers
*/
void pushVectorInsertInt(int *vector, int keyValue,
	int positionPush, int legthVector) {
	
  memcpy(&vector[positionPush + 1], &vector[positionPush],
		(legthVector - positionPush) * sizeof(int));

	vector[positionPush] = keyValue;
}

void pushVectorInsertLong(long *vector, long keyValue,
	int positionPush, int legthVector) {
	memcpy(&vector[positionPush + 1], &vector[positionPush],
		(legthVector - positionPush) * sizeof(long));

	vector[positionPush] = keyValue;
}

void contructorNodeBreak(node_break *res, int key,
	long offset, int leftChildren, int rightChildren){
	res->key = key;
	res->offset = offset;
	res->leftChildren = leftChildren;
	res->rightChildren = rightChildren;
}

node_break * createNodeBreak(int key, long offset, 
  int leftChildren, int rightChildren) {
  
	node_break *res = (node_break *)malloc(1 * sizeof(node_break));
	contructorNodeBreak(res, key, offset, leftChildren, rightChildren);
	return res;
}

//Passing a page return the key if the key exists

int searchPosition(node *buffer, int key){
  int middle, start, end;

	if (!buffer) return -1;
	if (buffer->cnt == 0) return -1;

  start = 0;
	end = buffer->cnt - 1;

  while (start <= end){
			middle = (start + end) / 2;
      printf("MIDDLE: %d\n", middle);
      if(key == buffer->keys[middle]){
        return middle;
      }
      else if (key < buffer->keys[middle]) {
				end = middle - 1;
			}	
      else{
				start = middle + 1;
			}
	}

  return -1;
}

int searchKeyInPage(node *buffer, int key){
  return searchPosition(buffer, key);
}

long searchOffsetInPage(node *buffer, int key, int *find){
  int position = searchPosition(buffer, key);

  if(position < 0){
    *find = -1; 
    return 0; //Long -1 is 18446744073709551615, so 0 is safer
  }

  *find = 1;
  return buffer->offsets[position];
}

/*
Here its very different from searchPosition
In searchPosition we need the position of the key in the page
Validating if the key exists {return position} else {return -1}
Here the base case is that the key don't exists in the page
And we're concerned to what position the key would be in the Btree
So, (position-1 < key) and (position+1 > key)
With bound check obvious
So if(key exists) {return -1} else{return position}
The opposite return and logic (almost)
Both use binary search at least
*/

int searchPage(node *buffer, int key){
	int middle, start, end;

	if (!buffer) return -1;
	if (buffer->cnt == 0) return 0;

	start = 0;
	end = buffer->cnt - 1;

	if (key < buffer->keys[0]) {
		return 0;
	}
  
	if (key > buffer->keys[end]) {
		return end + 1;
	}

	// Binary search
	else {
		while (start <= end) {
			middle = (start + end) / 2;
      
      if(key == buffer->keys[middle]){
        return -1;
      }

			if (key > buffer->keys[middle]) {     
				if (middle < buffer->cnt && key < buffer->keys[middle + 1]) {
          return middle + 1;
				}
				start = middle + 1;
				continue;
			}

			if (key < buffer->keys[middle]) {
				if (middle > 0 && key > buffer->keys[middle - 1]) {
					return middle;
				}
				end = middle - 1;
        
				continue;
			}
		}
	}

	return middle;
}

//Logical remove of the last pos of a leaf page
//Used in split

void removeLastPos(node *buffer, int *key, long* offset){
  buffer->cnt--;
  
  *key = buffer->keys[buffer->cnt];
  *offset = buffer->offsets[buffer->cnt];
}

/*
Its simple, when someone calls the split they pass the actual node they are
The quantity of pages, to be the last page and be created in memory
Then the code divides the actual node, sending a node_break to a level above

The logic that is hard to explain is the cutPoint
Basically, cutPoint takes half of the node and passes to the new node
If(key > originalPage[middle]) then insert on the new Page
But, to be "fair", we also copy less values to the new Page
Half half tecnically
Then we insert the key in the page (as a node because of recursion)
And takes the last position from the original page
*/
  
node_break *split_page( node *buffer, node_break *keyValue,
	                      FILE *arq, long *cntPages, int treeNode) {
  
	node *newPage = (node *)malloc(1 * sizeof(node));

	int cutPoint = (LIMIT-1) / 2;
	int keySave;
	long offsetSave;
  int flag = 0;

  
	if (keyValue->key > buffer->keys[cutPoint]) {
    flag = 1;
		cutPoint++;
  }

	int safeCut = buffer->cnt - cutPoint;
	// memcpy(destiny , origin , bites)
	// Copy of halt the page to the new page
	memcpy(&newPage->keys[newPage->cnt],&buffer->keys[cutPoint], safeCut * sizeof(int));
	
  memcpy(&newPage->offsets[newPage->cnt], &buffer->offsets[cutPoint], safeCut * sizeof(long));
  
	memcpy(&newPage->children[newPage->cnt], &buffer->children[cutPoint], (safeCut + 1) * sizeof(int));

	newPage->cnt = safeCut;
	buffer->cnt = cutPoint;
  buffer->leaf = newPage->leaf = buffer->leaf;
  
  if(flag){
    //Insert key already write the page
    //That why we need the else write the newPage
    insertKeyNode(*cntPages, newPage, keyValue, arq, cntPages);
  }else{
    insertKeyNode(treeNode, buffer, keyValue, arq, cntPages);
	  write(newPage, *cntPages, arq); 
  }

  removeLastPos(buffer, &keySave, &offsetSave);

	write(buffer, treeNode, arq); 
  
  //Return node break to the recursion
	contructorNodeBreak(keyValue, keySave, offsetSave, treeNode, *cntPages);

	*cntPages = *cntPages + 1;

  free(newPage);
	return keyValue;
}

// Return the position
int order_insert_leaf(node *buffer, int key, long offset) {
	int position = searchPage(buffer, key);

	if (position < 0) return position;

	pushVectorInsertInt(buffer->keys, key, position, buffer->cnt);
	pushVectorInsertLong(buffer->offsets, offset, position, buffer->cnt);

	return position;
}

int order_insert_node(node *buffer, node_break *newValue) {
	int position = order_insert_leaf(buffer, newValue->key, newValue->offset);

	if (position < 0) return position;

  //Push the children
	if (position != LIMIT - 1 && buffer->cnt - position > 0) {
		memcpy(
      &buffer->children[position + 2],
      &buffer->children[position + 1],
      ((buffer->cnt - position) * sizeof(int))
    );
	}

	buffer->children[position] = newValue->leftChildren;
	buffer->children[position + 1] = newValue->rightChildren;

	return position;
}

node_break *insertKeyLeaf(int treeNode,node *buffer, int key,long offset,FILE *arq,long *cntPages) {
	if (buffer->cnt == LIMIT - 1) {
    // Only works for LIMITS > 1, but its kinda obvious
		return split_page(buffer, createNodeBreak(key, offset,treeNode,*cntPages), arq, cntPages, treeNode);
	}
  
	int position = order_insert_leaf(buffer, key, offset);
  if(position < 0) return NULL;
  
  buffer->cnt++;

  write(buffer, treeNode, arq);
    
	return NULL;
}

node_break *insertKeyNode(int treeNode, node *buffer, node_break *key, FILE *arq, long *cntPages) {

  //Used in the split to ease things
  if(buffer->leaf == 1){
    return insertKeyLeaf(treeNode, buffer, key->key ,key->offset, arq, cntPages);
  }
  
  if (buffer->cnt == LIMIT - 1) { 
    // Only works for LIMITS > 1, but its kinda obvious
    return split_page(buffer, key, arq, cntPages, treeNode);
    
	}
  
  int position = order_insert_node(buffer, key);
  if(position < 0) return NULL;
  
  buffer->cnt++;

  write(buffer, treeNode, arq);
  
	return NULL;
}

//Look splitPage, searchPage and insertKeyLeaf
//Then you'll understang perfectly this logic
node_break *insertKeyRecursion(int treeNode, node *buffer,
	int key, long offset, FILE *arq, long *cntPages) {
	node_break *split_res = NULL;
  
	read(buffer, treeNode, arq);

	if (buffer->leaf) {
		split_res = insertKeyLeaf(treeNode, buffer, key, offset, arq, cntPages);
		return split_res;
	}

	int pagePosition = searchPage(buffer, key);

  if(pagePosition < 0) return NULL;
  
  pagePosition = buffer->children[pagePosition];
  
	read(buffer, pagePosition, arq);
  
	split_res = insertKeyRecursion(pagePosition, buffer, key, offset, arq, cntPages);

	if (split_res != NULL) {
		read(buffer, treeNode, arq);
		node_break *res = insertKeyNode(treeNode, buffer, split_res, arq, cntPages);

    //No more splits, so we can free
    if(res == NULL){
      free(split_res);
    }
    
    split_res = res;
	}
  
	return split_res;
}

int insertKey(node *buffer, int key, long offset, FILE *arq, long *cntPages) {
	if (*cntPages < 1) return -1;
  
  readRoot(buffer, arq);

	node_break *breakResult = insertKeyRecursion(0, buffer, key, offset, arq, cntPages);

	if (breakResult == NULL) return 0;

  // swap between the passed and the new one
  node *newRoot = (node *) calloc(1 , sizeof(node));

  readRoot(buffer,arq);

  //swap of the old root
  write(buffer, (int) *cntPages, arq);

  //swap of the pages childs 
  newRoot->children[0] = (int) *cntPages;
  newRoot->children[1] = breakResult->rightChildren;
  newRoot->leaf = 0;

  //New root can be seen as a leaf to make things easier
  insertKeyLeaf(0, newRoot,breakResult->key, breakResult->offset, arq, cntPages);

  *cntPages = *cntPages + 1;
  
  free(breakResult);
  free(newRoot);
  fflush(arq);  
  
	return 0;
}

long searchOffsetInTree(node *buffer, FILE *arq, int key, int *find){
  int pagePosition = 0;
  readRoot(buffer, arq);

  //Most of the keys are in the leavessearchOffsetInTree
  while(!buffer->leaf){
  	read(buffer, pagePosition, arq);
    
    pagePosition = searchPage(buffer, key);

    //Search page-find == -1
    if(pagePosition < 0) break;
    
    pagePosition = buffer->children[pagePosition];
  }
  
  return searchOffsetInPage(buffer, key, find);
}