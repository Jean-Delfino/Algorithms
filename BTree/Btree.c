#include "Btree.h"

node_break* insertKeyNode(int treeNode, node* buffer, node_break* key, FILE *arq, long *cntPages);

void read(node *buffer , int localization , FILE *arq){
  fseek(arq , localization*page , SEEK_SET);

  fread(buffer , sizeof(node) , 1 , arq);
}

void write(node *buffer , int localization , FILE *arq){
  fseek(arq , localization*page , SEEK_SET);

  fwrite(buffer , sizeof(node) , 1 , arq);
}

void pushVectorInsertInt(int* vector, int keyValue, int positionPush, int legthVector){
  memcpy( &vector[positionPush + 1],
          &vector[positionPush],
          (legthVector - positionPush) * sizeof(int));
  
  vector[positionPush]  = keyValue;
}

void pushVectorInsertLong(long* vector, long keyValue, int positionPush, int legthVector){
  memcpy( &vector[positionPush + 1],
          &vector[positionPush],
          (legthVector - positionPush) * sizeof(long));
  
  vector[positionPush]  = keyValue;
}

void contructorNodeBreak(node_break *res,int key, long offset, int leftChildren, int rightChildren){
  res->key = key;
  res->offset = key;
  res->leftChildren = leftChildren;
  res->rightChildren = rightChildren;
}

node_break* createNodeBreak(int key, long offset, int leftChildren, int rightChildren){
  node_break* res = (node_break*) malloc(1 * sizeof(node_break));
  contructorNodeBreak(res,key,offset,leftChildren,rightChildren);
  return res;
}

int searchPage(node *buffer , int key){
  int middle, start, end;

  if(!buffer) return -1;
  if(buffer->cnt == 0) return 0;
  
  start = 0;
  end = buffer->cnt-1;

  if(key < buffer->keys[0]){
    return 0;
  }
  if(key > buffer->keys[end]){
    return end;
  }

  //Binary search
  else{
    while(start < end){
      middle = (start+end)/2; 

      if(key > buffer->keys[middle]){
        if(middle > 0 && buffer->keys[middle-1] < key){
          return middle + 1;
        }
        start = middle +1;
        continue;
      }

      if(key < buffer->keys[middle]){
        if(middle < buffer->cnt && key > buffer->keys[middle+1]){
          return middle;
        }
        end = middle - 1;
        continue;
      } 
    }
  }

  return middle;
}

//Its simple, when someone calls the split they pass the actual node they are
//The quantity of pages, to be the last page and be created in memory
//Then the code divides the actual node, sending a node_break to a level above

node_break* split_page(node *buffer, node_break *keyValue, FILE *arq, long *cntPages, int treeNode){
  node *newPage = (node*) malloc(1 * sizeof(node));

  int cutPoint = LIMIT / 2;
  int keySave = buffer->keys[cutPoint];
  long offSetSave = buffer->offsets[cutPoint];

  if(keyValue->key > buffer->keys[cutPoint]){
    insertKeyNode(*cntPages, newPage, keyValue, arq, cntPages);
    cutPoint++;
  }

  int safeCut = buffer->cnt - cutPoint;
  //memcpy(destiny , origin , bites)
  //Copy of halt the page to the new page
  memcpy(&newPage->keys[newPage->cnt] , &buffer->keys[cutPoint] , 
    safeCut * sizeof(int));
  memcpy(&newPage->offsets[newPage->cnt] , &buffer->offsets[cutPoint] , 
    safeCut * sizeof(long));
  memcpy(&newPage->children[newPage->cnt] , &buffer->children[cutPoint] , 
    (safeCut +1 )*sizeof(int));

  newPage->cnt = safeCut + newPage->cnt;
  buffer->cnt = cutPoint;

  write(newPage , *cntPages , arq);
  write(buffer , treeNode , arq);

  contructorNodeBreak(keyValue,keySave, offSetSave, treeNode, *cntPages);
  
  *cntPages = *cntPages + 1;
  return keyValue;
}

//Return the position
int order_insert_leaf(node *buffer, int key , long offset){
  int position = searchPage(buffer, key);

  if(position < 0) return position; 

  pushVectorInsertInt(buffer->keys, key, position, buffer->cnt);
  pushVectorInsertLong(buffer->offsets, offset, position, buffer->cnt);

  return position;
} 

int order_insert_node(node *buffer, node_break* newValue ){
  int position = order_insert_leaf(buffer, newValue->key , newValue->offset);

  if(position < 0) return position;

  if(position != LIMIT-1 && buffer->cnt - position > 0){
    memcpy(&buffer->children[position+2],
     &buffer->children[position+1],
     ((buffer->cnt - position) * sizeof(int)));
  }

  buffer->children[position] = newValue->leftChildren;
  buffer->children[position+1] = newValue->rightChildren;

  return position;
}

node_break* insertKeyLeaf(int treeNode , node* buffer, int key , long offset , FILE *arq, long *cntPages){
  if(buffer->cnt == LIMIT - 1){ //Only works for LIMITS > 1, but its kinda obvious
    return split_page(buffer,
    createNodeBreak(key, offset, treeNode,*cntPages),
    arq , cntPages, treeNode);
  }
  int position = searchPage(buffer, key);

  return NULL;
}


node_break* insertKeyNode(int treeNode, node* buffer, node_break* key, FILE *arq, long *cntPages){
  return NULL;
}

node_break* insertKeyRecursion(int treeNode , node *buffer , int key , long offset , FILE *arq, long *cntPages){
  read(buffer, treeNode, arq);
  node_break* split_res = NULL;

  if(buffer->leaf){
    split_res = insertKeyLeaf(treeNode, buffer, key,offset, arq, cntPages);
    return split_res;
  }

  int pagePosition = buffer->children[searchPage(buffer , key)];
  read(buffer, pagePosition, arq);
  insertKeyRecursion(pagePosition, buffer, key, offset, arq, cntPages);

  if(split_res){
    read(buffer, treeNode, arq);
    split_res = insertKeyNode(treeNode, buffer, split_res, arq, cntPages);
  }

  return split_res;
}

int insertKey(node* buffer, int key , long offset , FILE *arq , long *cntPages){
  if(*cntPages < 1) return -1;

  node_break* breakResult = insertKeyRecursion(0, buffer, key , offset, arq, cntPages);

  if(breakResult){
    //swap between the passed and the new one

    //simply change the reference from 0 to the passed
  }

  return 0;
}
