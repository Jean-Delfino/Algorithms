#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMA ','
#define ARCHIVE "alunos.bin"
#define LAST_WANTED 10

typedef struct Student{
  int nUsp;
  char *name;
  char *course;
  float score;
}Student;

char* stringReadCSV(char separator, int *end){
  char c;
  char *stringRet;
  int size = -1; //Look line 24 and it'll be obvious

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
  }while(c != separator && c != '\n');

  stringRet[size] = '\0'; //Overwrites the separator

  //printf("%s\n", stringRet);

  return stringRet;
}

int intReadCSV(char separator, int *end){
  return atoi(stringReadCSV(COMMA , end));
}

float floatReadCSV(char separator, int *end){
  return atof(stringReadCSV(COMMA , end));
}

void writeString(FILE *arq, char* data){
  int size = strlen(data);
  fwrite(&size, 1, sizeof(int), arq);
  fwrite(data, (size), sizeof(char), arq);
}

void writeInEndFile(FILE *arq, Student* alu){
  fseek(arq, 0, SEEK_END);
  fwrite(&alu->nUsp, 1, sizeof(int), arq);
  writeString(arq, alu->name);
  writeString(arq, alu->course);
  fwrite(&alu->score, 1, sizeof(float), arq);
}

char *readString(FILE *arq){
  int size;
  char *stringRet;
  
  fread(&size, 1, sizeof(int), arq);

  stringRet = malloc((size+1) * sizeof(char));
  fread(stringRet, size, sizeof(char), arq);
  
  stringRet[size] = '\0';
  
  return stringRet;
}

void readInFile(FILE *arq,Student *buff, long position){
  fread(&buff->nUsp, 1, sizeof(int) , arq);
  
  buff->name = readString(arq);
  buff->course = readString(arq);
  
  fread(&buff->score, 1, sizeof(float) , arq);
}

void openFile(FILE *arq, char *nameFile) {
    if (arq == NULL) {
        arq = fopen(nameFile, "w");
        fclose(arq);
        arq = NULL;
    }
}

void printStudent(Student stu){
  printf("nUSP: %d\n" , stu.nUsp);
  printf("Nome: %s\n" , stu.name);
  printf("Curso: %s\n" , stu.course);
  printf("Nota: %.2f" , stu.score);
}

void readSeveralArchives(FILE *arq, long pos, int size){
  int i;
  Student *class = malloc(1 * sizeof(Student));
  
  fseek(arq, pos, SEEK_SET);
  
  for(i = 0; i < size - 1; i++){
    readInFile(arq, class, ftell(arq));
    printStudent(*class);
    printf("\n\n");
  }
  
  readInFile(arq, class, ftell(arq));
  printStudent(*class);

  free(class);
}


int main(void) {
  int end = 0, tam = 0, i;
  FILE *arq;
  Student* class;
  Student* buff;
  
  long* position; //Aux to the read in the file

  do {
	    arq = fopen(ARCHIVE, "r+");
	    if (arq== NULL) openFile(arq, ARCHIVE);
	} while(arq == NULL);

  class = malloc(1 * sizeof(Student));
  
  position = malloc(2 * sizeof(long));

  position[0] = ftell(arq); //Position of the first
  
  while(!end){
    
    class->nUsp = floatReadCSV(COMMA , &end);
    class->name = stringReadCSV(COMMA , &end);
    class->course = stringReadCSV(COMMA , &end);
    class->score = floatReadCSV(COMMA , &end);

    //printStudent(*class);

    writeInEndFile(arq, class);
    fflush(arq);
    
    tam++;
    
    position[tam] = ftell(arq);
    position = realloc(position, (tam+2) * sizeof(long));
    
    free(class->name);
    free(class->course);
  }

  if(tam < LAST_WANTED){
    return 0;
  }

  
  //readSeveralArchives(arq, position[0], 2);
  readSeveralArchives(arq, position[tam-LAST_WANTED], LAST_WANTED);
      
  free(class);
  fclose(arq);

  return 0;
}
