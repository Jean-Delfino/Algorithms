//2 years old code being remade in current time

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define ordem 4
//#define ordem 240
#define pag 4096
#define CAMPOSVARIAVEIS 3

typedef struct{
  int contador; //nro de chaves armazenadas
  int chaves[ordem-1]; //assumindo chaves char
  int filhos[ordem]; // armazena o RRN dos filhos
  long offsets[ordem-1]; // armazena o RRN dos filhos
  char folha;
} PAGINA; 

typedef struct {
	int numUSP; // 4
	char *nome;   
	char *sobrenome; 
	char *curso;  
	float nota;       // 4
} tipoAluno;

typedef struct{
 int Qtdpag; // começa com 1
 int root;
} DESCRITOR; 

void insertKeyNode(PAGINA *node , int key , long offset , int sonleft ,FILE *arq, int sonright , long *vetFather , int pos , DESCRITOR *btree);
void insertKeyLeaf(PAGINA *node , int key , long offset , long *vetFather , int pos , FILE *arq , DESCRITOR *btree);


void printarNo(PAGINA *buffer){
  int i;
  printf("Quantidade = %d\n" , buffer->contador);

  for(i = 0 ; i < ordem - 1 ; i++){
    printf("chave = %d\noffsets = %lu\n" , buffer->chaves[i] , buffer->offsets[i]);
  }

  for(i = 0 ; i < ordem; i++){
    printf("filhos = %d\n" , buffer->filhos[i]);
  }

  printf("folha = %d\n\n" , (int) buffer->folha);
}

void looseOrder(PAGINA *node){
	int i , aux;
  long aux2;
  //printf("entrou ordena aqui\n");
	for(i = node->contador ; i > 0 ; i--){
		if(node->chaves[i] < node->chaves[i-1]){
			aux = node->chaves[i-1];
			node->chaves[i-1] = node->chaves[i];
			node->chaves[i] = aux;

			aux2 = node->offsets[i-1];
			node->offsets[i-1] = node->offsets[i];
			node->offsets[i] = aux2;

		}

		else{
			break;
		}
	}	
}	

void hardOrder(PAGINA *node , int sonright , int sonleft){
  int i , aux , pos = 0;
  long aux2;

  for(i = node->contador ; i > 0 ; i--){
		if(node->chaves[i] < node->chaves[i-1]){

			aux = node->chaves[i-1];
			node->chaves[i-1] = node->chaves[i];
			node->chaves[i] = aux;

			aux2 = node->offsets[i-1];
			node->offsets[i-1] = node->offsets[i];
			node->offsets[i] = aux2;

		}

		else{
			break;
		}
  }
  pos = i;

  //memcpy(destino , origem , qtd_bites)
  if(pos != ordem-1 && node->contador - pos > 0){
    memcpy(&node->filhos[pos+2] , &node->filhos[pos+1] ,  ((node->contador - pos ) * sizeof(int)));
  }

  node->filhos[pos] = sonleft;
  node->filhos[pos+1] = sonright;
	

}	


void read(PAGINA *buffer , int localization , FILE *arq){
  fseek(arq , localization*pag , SEEK_SET);

  fread(buffer , sizeof(PAGINA) , 1 , arq);
}

void write(PAGINA *buffer , int localization , FILE *arq){
  fseek(arq , localization*pag , SEEK_SET);

  fwrite(buffer , sizeof(PAGINA) , 1 , arq);

}


void printarArvore(FILE *arq , DESCRITOR *btree){
  int i;
  PAGINA *buffer;

  buffer = (PAGINA*) calloc(1,sizeof(PAGINA));

  printf("\nDescritor\n\nquantidade de nos = %d\nraiz = %d\n" , btree->Qtdpag , btree->root);
  for(i = 0 ; i < btree->Qtdpag ; i++){
    read(buffer , i , arq);
    printf("Pagina %d\n" , i);
    printarNo(buffer);
  }
}


int searchPag(PAGINA *buffer, int key , FILE *arq){
  
  int inicio, meio, fim;
  recomeco:

  inicio = 0;
  fim = buffer->contador;

  if( !buffer->folha && key < buffer->chaves[0]){
    read(buffer , buffer->filhos[0] , arq);
    goto recomeco;
  }
  else if(!buffer->folha && key > buffer->chaves[buffer->contador-1] ){
    read(buffer , buffer->filhos[buffer->contador] , arq);
    goto recomeco;
  }
  else{
    while(inicio < fim){
      meio = (inicio+fim)/2; 

      if(!buffer->folha && buffer->chaves[meio] > key && meio >0 && buffer->chaves[meio-1] < key){
        read(buffer , buffer->filhos[meio] , arq);
        goto recomeco;
      }

      else if(!buffer->folha && buffer->chaves[meio] < key && meio < buffer->contador && buffer->chaves[meio+1] > key){
        read(buffer , buffer->filhos[meio+1] , arq);
        goto recomeco;
      }

      else if(key > buffer->chaves[meio]){
        inicio = meio +1;
      }

      else if(key < buffer->chaves[meio]){
        fim = meio - 1;
      }
      else{
        printf("Achei\n");
        return buffer->offsets[meio];
      }
      
    }
  }
  printf("Chave não existente\n");
  return -1;
}

void insertKeySplitNode(PAGINA *node , int key , long offset , int sonleft, int sonright){
  //Eu sei que caso essa função ocorra não terá split, logo ela é uma simplificação das já existentes
  node->chaves[node->contador] = key;
  node->offsets[node->contador] = offset;
  hardOrder(node , sonright , sonleft);	
  node->contador++;

}

void split(PAGINA *node, int nodepag ,int key, long offset ,DESCRITOR *btree , FILE *arq , long *vetFather , int pos , int sonleft , int sonright){
	PAGINA *newp , *newestag;

  //Qual é o meio do vetor baseado na inserção de um novo valor
	int middlecut;
  //Qual no inserir
  int flag=0;
  //Em qual pagina será escrito os nós inicializados
  int reference = btree->Qtdpag;
  //Variaveis auxiliares para insercao
  long auxoff;
  int aux;
  
  newp = (PAGINA*) calloc(1,sizeof(PAGINA));
  //Sempre o novo é a segunda metade
  middlecut = (int) (ordem-1)/2;
  
  if(key < node->chaves[middlecut]){
    flag = 1; 
  }
  //É copiado menos valores por conta do balanceamento
  else{
    middlecut++;
  }

    //Variaves auxiliar para o memcpy
    int safecut = node->contador - middlecut;

    //memcpy(destino , origem , qtd_bites)
    //Copiando os valores do node original para o novo nó
    memcpy(newp->chaves , &node->chaves[middlecut] , safecut * sizeof(int));
    memcpy(newp->offsets , &node->offsets[middlecut] , safecut * sizeof(long));
    memcpy(newp->filhos , &node->filhos[middlecut] , (safecut +1 )*sizeof(int));

    //isso que irá gerar a deleta dos valores do nó propriamente dizendo (remoção lógica)
    newp->contador = node->contador - middlecut;
    node->contador = middlecut;
    //Ao quebrar um nó o novo nó será igual ao nó quebrado em tipo (folha ou pai)
    newp->folha = node->folha;

    //Valor será colocado na primeira metade (node original)
    if(flag){
      insertKeySplitNode(node , key , offset , sonleft, sonright);
    }
    //Valor será colocado no novo nó criado
    else{
      insertKeySplitNode(newp , key , offset , sonleft,sonright);
    }

    //Escreve o node na memória
    write(node , nodepag , arq);

    //Acontece a escrita e deleta do novo nó
    //Isso para em caso de N Overflows seja apenas gastado um 0(1) de espaço (as variaveis ints + variaveis longs + as variaveis ponteiro não mallocadas)
    write(newp , reference , arq);

    //São salvos os valores de newp
    aux = newp->chaves[0];
    auxoff = newp->offsets[0];

    free(newp);

    //Acontece a quebra da raiz
    if(nodepag == btree->root){
      //Uma nova pagina que sera utilizada como pai
      PAGINA *newroot;		
      
      newroot = (PAGINA*) calloc(1 , sizeof(PAGINA));
      btree->root = reference + 1;

      //Insercao do primeiro valor do no criado
      newroot->chaves[0] = aux;
      newroot->offsets[0] = auxoff;
      newroot->filhos[0] = nodepag;
      newroot->filhos[1] = reference;
      //Troca o estado da folha
      newroot->folha = 0;
      newroot->contador++;

      write(newroot , reference+1 , arq);

      btree->Qtdpag+=2; 
      free(newroot);
    }
    
    //Overflow
    else{
      //É lido o pai do no atual
      read(node , vetFather[pos-1] , arq);
      
      //Incremento da pagina para a escrita dos nos ficarem corretas mesmo no overflow
      btree->Qtdpag++;
      insertKeyNode(node , aux , auxoff , nodepag ,arq, reference , vetFather , pos-1 , btree);
    }

    //Retirada do valor que foi splitado para os nós superiores
    newestag = (PAGINA*) calloc(1,sizeof(PAGINA));
    read(newestag , reference , arq);

    memcpy(newestag->chaves , &newestag->chaves[1]  , (newestag->contador-1) * sizeof(int));
    memcpy(newestag->offsets , &newestag->offsets[1] , (newestag->contador-1) * sizeof(long));
    memcpy(newestag->filhos , &newestag->filhos[1] , (newestag->contador) * sizeof(int));
    newestag->contador--;

    write (newestag ,reference, arq);
    free(newestag);
}


void insertKeyLeaf(PAGINA *node , int key , long offset , long *vetFather , int pos , FILE *arq , DESCRITOR *btree){

  if(node->contador == ordem - 1){
    split(node,vetFather[pos] , key, offset ,btree , arq ,vetFather , pos , -1 , -1);
  }
  //Insercao no final
  else{
    node->chaves[node->contador] = key;
    node->offsets[node->contador] = offset;
    looseOrder(node);
    node->contador++;
    write(node , vetFather[pos] , arq);
  }
}

void insertKeyNode(PAGINA *node , int key , long offset , int sonleft ,FILE *arq, int sonright , long *vetFather , int pos , DESCRITOR *btree){

  if(node->contador == ordem - 1){
    split(node, vetFather[pos], key, offset, btree, arq, vetFather, pos, sonleft, sonright);
  }
  //Insercao no final
  else{
    node->chaves[node->contador] = key;
    node->offsets[node->contador] = offset;
    hardOrder(node , sonright , sonleft);	
    node->contador++;
    write(node , vetFather[pos] , arq);
  }
}

int insertKey(PAGINA *buffer , int key , long offset , FILE *arq , DESCRITOR *btree){
  int inicio, meio,fim;

  long *vetordePais;
  int tam = 0;

  //Guarda quem é a raiz
  vetordePais = (long*) malloc(1*sizeof(long));
  vetordePais[0] = btree->root;

  //O primeiro IF é para verificar se já está em uma folha , podendo assim inserir
  //Basicamente o 2,3,4 e 5 IF's servem para guardar qual foram os nós que foram percorridos
  //Os ultimos dois são para uma busca binária
  recomeco:

  inicio = 0;
  fim = buffer->contador-1;

  if(buffer->folha){
    insertKeyLeaf(buffer ,key ,offset , vetordePais , tam , arq , btree);
    return 1;
  }

  if(key < buffer->chaves[0]){
    tam++;
    vetordePais = (long*) realloc(vetordePais, tam+1*sizeof(long));
    vetordePais[tam] = buffer->filhos[0];

    read(buffer , buffer->filhos[0] , arq);

    goto recomeco;
  }

  else if(key > buffer->chaves[buffer->contador-1]){
    tam++;
    vetordePais = (long*) realloc(vetordePais, tam+1*sizeof(long));
    vetordePais[tam] = buffer->filhos[buffer->contador];

    read(buffer , buffer->filhos[buffer->contador] , arq);

    goto recomeco;
  }

  else{
    while(inicio < fim){
      meio = (inicio+fim)/2; 

      if(buffer->chaves[meio] > key && meio > 0 && buffer->chaves[meio-1] < key){
        tam++;
        vetordePais = (long*) realloc(vetordePais, tam+1*sizeof(long));
        vetordePais[tam] = buffer->filhos[meio];

        read(buffer , buffer->filhos[meio] , arq);

        goto recomeco;
      }

      else if(buffer->chaves[meio] < key && meio < buffer->contador && buffer->chaves[meio+1] > key){
        tam++;
        vetordePais = (long*) realloc(vetordePais, tam+1*sizeof(long));
        vetordePais[tam] = buffer->filhos[meio+1];

        read(buffer , buffer->filhos[meio+1] , arq);

        goto recomeco;
      }

      else if(key > buffer->chaves[meio]){
        inicio = meio +1;
      }

      else if(key < buffer->chaves[meio]){
        fim = meio - 1;
      }
      
    }
  }

  return 0;
}


void criaArquivo(FILE *df) {
    if (df== NULL) {
        df= fopen("alunos.dad", "w");        /* Se nao existir, cria o arquivo de dados */
        fclose(df);
        df= NULL;
    }
}

void criaArquivoArvore(FILE *df) {
    if (df== NULL) {
        df= fopen("arvore.bin", "w");        /* Se nao existir, cria o arquivo de dados */
        fclose(df);
        df= NULL;
    }
}

void criaArquivoArvoreDesc(FILE *df) {
    if (df== NULL) {
        df= fopen("descritor.txt", "w");        /* Se nao existir, cria o arquivo de dados */
        fclose(df);
        df= NULL;
    }
}

void funcaolefrase(char *retorno , FILE *df){
  int i=0;
  char a = a;
  //puts("COMECA LER");
  retorno =(char*) malloc(1*sizeof(char*));
  while(1){
    scanf("%c" ,&a);

    if(a != '\n'){
      retorno = (char*) realloc(retorno,i+1*sizeof(char));
      retorno[i] = a;
      i++;
    }
    else{
      retorno[i] ='\0';
      break;
    }
  }
  fprintf(df, "%d%s" ,(int) strlen(retorno) ,retorno);
  free(retorno);
}

void imprimeRegistro(int pesquisa , FILE *df){
  char *frase = NULL;
  float c;
  int i , tam;
  unsigned char j;

  fscanf(df,"%d" , &pesquisa);
  fread(&j,sizeof(unsigned char),1,df);
  fscanf(df , "%d|%d|" , &i , &tam);

  if(j == '|' ){
    printf("%d " , i);
    for(i = 0 ; i < CAMPOSVARIAVEIS; i++){
      fscanf(df,"%d" ,&tam);
      frase = (char*) realloc(frase,tam+1*sizeof(char));
      fread(frase,tam,1,df);
      frase[tam] = '\0';
      printf("%s " , frase);
    }

    fscanf(df,"%d|" ,&i);
    tam = 0;
    while(1){
      frase = (char*) realloc(frase,tam+1*sizeof(char));
      fread(&frase[tam],1,1,df);

      if(frase[tam] == '|'){
        frase = (char*) realloc(frase,tam*sizeof(char));
        c = atof(frase);
        printf("%.2f\n" , c);
        break;
      }
      tam++;
    }
  }
  else{
    printf("Registro removido\n");
  }
  free(frase);
}

void insereRegistro(FILE *df , PAGINA *buffer , FILE *arq , DESCRITOR *btree){
  tipoAluno reg;
  printf("Insira numero usp , nome , sobrenome, curso e a nota do indivíduo\n");
  
  fseek(df , 0 , SEEK_END);

  scanf("%d" , &reg.numUSP);
  scanf("\n");
  insertKey(buffer , reg.numUSP, ftell(df) ,arq ,btree);

  fprintf(df, "|%d|" ,(int) sizeof(int));
  fprintf(df, "%d|" ,reg.numUSP);

  funcaolefrase(reg.nome , df);
  funcaolefrase(reg.sobrenome , df);
  funcaolefrase(reg.curso , df);
  
  scanf("%f" , &reg.nota);
  
  fprintf(df, "%d|%f" ,(int) sizeof(float) ,reg.nota);

}

void test(PAGINA *buffer, FILE *arq , DESCRITOR *btree){
  int key;
  long offset;

  printf("Insira chave e offset\n");

  scanf("%d %lu" , &key , &offset);

  read(buffer , btree->root , arq);
  insertKey(buffer , key , offset , arq , btree);
  //printarNo(buffer);
}

int main(void) {
  FILE *df , *arq , *desc;
  PAGINA *buffer;
  DESCRITOR *btree;
  int operation;
  int flag;

  btree = (DESCRITOR*) malloc(1 * sizeof(DESCRITOR));
  buffer = (PAGINA*) calloc(1 , sizeof(PAGINA));

  do {
	    df= fopen("alunos.dad", "r+");
	    if (df== NULL) criaArquivo(df);
	} while(df== NULL);

  do {
	    arq= fopen("arvore.bin", "r+");
	    if (arq== NULL) criaArquivoArvore(arq);
	} while(arq== NULL);

  do {
	    desc= fopen("descritor.txt", "r+");
	    if (desc== NULL) criaArquivoArvoreDesc(desc);
	} while(desc== NULL);

  fseek(desc, 0, SEEK_END);

  if(!ftell(desc)){
    btree->Qtdpag = 1;
    btree->root = 0;
    buffer->folha = 1;
  }

  else{
    fseek(desc, 0, SEEK_SET);
    fscanf(desc , "%d %d" , &btree->Qtdpag , &btree->root);
  }
  do{
    printf("Operacoes:\n0- Fechar o progrma\n1- Gravar\n2- Pesquisar\n3- Printar todos os nós da árvore btree\n");
    scanf("%d" , &operation);

    if(operation == 1){
      //test(buffer, arq ,  btree);
      insereRegistro(df , buffer ,arq , btree);
    }

    else if(operation == 2){
      printf("Insira uma chave para pesquisa\n");
      scanf("%d" ,&flag);
      flag = searchPag(buffer , flag ,arq);
      if(flag != -1){
        fseek(df , flag , SEEK_SET);
        imprimeRegistro(flag , df);
      }
    }

    else if(operation ==3){
      //printar arvore
      printarArvore(arq , btree);
    }
    else if(operation == 0){
      fseek(desc , 0 ,SEEK_SET);
      fprintf(desc , "%d %d" , btree->Qtdpag , btree->root);
      break;
    }

  }while(operation > 0);

  return 0;
}
