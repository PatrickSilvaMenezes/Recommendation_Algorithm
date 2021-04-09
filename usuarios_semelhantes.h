#ifndef _USUARIOS_SEMELHANTES_
#define _USUARIOS_SEMELHANTES_
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "leitura_Base_De_Dados.h"
#define idMovie 494
#define hash(v, M) (v % M)

typedef struct TipoPilha Pilha;
typedef struct TipoBloco1 Bloco1;
typedef struct TipoBloco2 Bloco2;
typedef struct TipoLista Lista;
typedef struct TipoItem Item;
typedef struct DataTable Data;
typedef struct HashTable HashTable;
typedef struct MovieUser Movie;




struct DataTable{
	int key;
	int value[10];
	int count; //necessário apenas para a política LRU
};
struct HashTable{
	Data *table;
	int M;
};

void Initialize(HashTable *h, int M){
	h->table = (Data*) malloc (M * sizeof(Data));
	for(int i=0; i<M; i++){
		h->table[i].key   = -1;
		for(int j=0;j<10;j++)
		{
			h->table[i].value[j] = 0;
		}
		h->table[i].count = 0;
	}

	h->M = M;
}
void Imprime(HashTable *h){
	for(int i=0; i<h->M; i++)
	{
		printf("KEY:%d - VALUE:  ",h->table[i].key);
		for(int j=0;j<10;j++)
		{
			printf("%d   ", h->table[i].value[j]);
		}
		printf("\n");
	}
}
void ImprimeChaves(HashTable *h)
{
	for(int i=0;i<h->M;i++)
	{
		printf("KEY:%d  ",h->table[i].key);
	}
}

int LRUPolicy(HashTable *h){
	int key;

	key = 0;
	for(int i=0; i<h->M; i++){
		if(h->table[i].count < h->table[key].count)
			key = i;
		h->table[i].count = 0;
	}

	return key;
}
void Insert(HashTable *h, int key, int *best_movies){
	int idx = hash(key, h->M);
	int aux = idx;
	
	while (h->table[idx].key != -1){
		idx = (idx + 1) % h->M;
		if (idx == aux){
			idx = LRUPolicy(h);
			break;
		}
	}	
	
	//printf("IDX:%d\n", idx);

	h->table[idx].key   = key;
	for(int i=0;i<10;i++)
	{

		h->table[idx].value[i] = best_movies[i];
	}
	h->table[idx].count = 1;
}
 



//LISTA
struct TipoItem
{
	int filme;
	int nota;
};
struct TipoBloco1
{
	Item dado;
	Bloco1 *prox;
};
struct TipoLista
{
	Bloco1 *primeiro;
	Bloco1 *ultimo;
};

Lista* createLista()
{
	Lista *L = (Lista*) malloc (sizeof(Lista));
	L->primeiro = (Bloco1*) malloc(sizeof(Bloco1));
	L->ultimo = L->primeiro;
	L->primeiro->prox = NULL;
	return L;
}
void Listar(Item dado,Lista *L)
{
	L->ultimo->prox = (Bloco1*)malloc(sizeof(Bloco1));
	L->ultimo = L->ultimo->prox;
	L->ultimo->dado = dado;
	L->ultimo->prox= NULL;
}
void FLVazia(Lista *L)
{
	L->primeiro = (Bloco1*)malloc(sizeof(Bloco1));
	L->ultimo = L->primeiro;
	L-> primeiro->prox = NULL;
}
void ImprimeLista(Lista *L)
{
	Item dado;
	
	Bloco1 *aux = L ->primeiro->prox;

	while (aux->prox != NULL)
	{
		printf("Filme: %d \n", aux->dado.filme);
		printf("Nota: %d \n",aux->dado.nota );

		aux = aux->prox;
	}

}

//PILHA
struct TipoBloco2
{
	int dado;
	Bloco2 *prox;
};
struct TipoPilha
{
	Bloco2 *fundo;
	Bloco2 *topo;
};

void FPVazia(Pilha *p)
{
	p->fundo =(Bloco2*)malloc(sizeof(Bloco2));
	p->fundo->prox = NULL;
	p->topo = p->fundo;
}

Pilha* createPilha()
{
	Pilha* P=(Pilha*) malloc(sizeof(Pilha));
	P -> topo = P->fundo;
	P-> topo -> prox = NULL;
	P-> fundo =NULL;
	return P;
}
void Empilha(int dado,Pilha *p)
{
	Bloco2 *aux;
	aux = (Bloco2*)malloc(sizeof(Bloco2));
	aux->dado = dado;
	aux->prox = p->topo;
	p->topo=aux;
}
void ImprimePilha(Pilha *P)
{
	
	Bloco2 *aux = P ->topo;
	while (aux->prox != NULL)
	{
		printf("Usuario: %d \n", aux->dado);
		aux = aux->prox;
	}
}

void Recomendation(FILE *file,char *address,int **mat)
{
	//variaveis e vetores
	char linha[100];
	const char delimitador[] = ",";
	char *token;
	int line=0,line_aux=0;
	int Notas_User[idMovie];
	
	Pilha *P= createPilha();
	FPVazia(P);

	Pilha *P2 = createPilha();
	FPVazia(P2);

	Lista *L= createLista();
	FLVazia(L);

	Item dado;
	
	int data_UsersLikely;
	//passando a matriz de entrada para uma matriz aux
	
	file = fopen(address,"r");
    
	if (file == NULL)
		printf("Erro! Não foi possível abrir o arquivo!");
	//inicializando o vetor de nota do usuario de entrada

	
	for(int i=0;i<idMovie;i++)
	{
		Notas_User[i]=0;

	}
	
	// obtendo o vetor com indice de id de filmes e com as notas em cada indice

    // pegar o vetor com os ids dos filmes do usuario de entrada para servir como chave da tabela hash
   	int c;
	printf("=============================Notas dos Filmes do Usuario de Entrada==============================\n");
    while(fgets(linha,sizeof(linha),file) != NULL)
	{
		token = strtok(linha, delimitador);
		if(atoi(token)<idMovie)
		{

			line = atoi(token);
			
			token = strtok(NULL,delimitador);//parei quando achei o primeiro delimitador, após isso poderei seguir para o proximo elemento
			c++;
		}
	
				Notas_User[line] = atoi(token); 
		
		
		printf("Usuário de Entrada:1 || Filme: %d || Nota: %d \n",line,Notas_User[line]);
		

	}
	fclose(file);
			
	

	printf("\n\n==================================================================================================\n\n");
	//obtendo os filmes da base netflix

	int user_semelhantes[idUser];
	int movie_semelhantes[idFilme];
	int pesos[idUser];
	for(int i=0;i<idUser;i++)
	{
		user_semelhantes[i]=0;
		pesos[i]=0;
		movie_semelhantes[i]=0;
		
	}
	int cont=0,soma=0;
	//agora irei comparar os filmes desse usuario com os que os usuarios da matriz viram e obter os usuarios
	//semelhantes
	printf("=============================Usuarios Semelhantes e Seus Pesos=================================================\n");
	for(int i=0;i<idUser;i++)
	{
		cont=0;
		soma =0;
		for(int j=0;j<idFilme;j++)
		{
			// se for diferente de 0 a nota dada pelo usuario de entrada e o da matriz quer dizer que ambos viram
			if(mat[i][j]!=0 && Notas_User[j]!=0)
			{
				cont++;
				soma = soma+mat[i][j];
			}
		}
		//filtrando baseado em uma constante arbitraria os usuarios semelhantes que viram pelo menos 1  filme igual ao
		//usuario de entrada
		if(cont>=1)
		{

			user_semelhantes[i]=i;

			pesos[i] = soma;
			data_UsersLikely = user_semelhantes[i];
			Empilha(data_UsersLikely,P);
			printf("Usuario %d avaliou o mesmo filme %d vezes, Peso:%d\n",user_semelhantes[i],cont,pesos[i]);
		}
		

	}
	
	//imprimindo a pilha com usuarios semelhantes
	printf("===================================================================================================\n\n\n");

	printf("===================================Pilha com Usuarios Semelhantes==================================\n");	
	ImprimePilha(P);
	printf("====================================================================================================\n");

	int data_UsersSort;
	int   aux=0,aux2;
	// Metodo usado para ordenar os usuarios com base em seus pesos Bubble Sort , mais pratico e simples de se usar 

  for (int j = 1; j < idUser; j++) {
    for (int i = 0; i < idUser - 1; i++) {
      if (pesos[i] > pesos[i + 1]) {
        aux = pesos[i];
        pesos[i] = pesos[i + 1];
        pesos[i + 1] = aux;
        aux2 = user_semelhantes[i];
        user_semelhantes[i] = user_semelhantes[i+1];
        user_semelhantes[i+1]= aux2;
      }
    }
 
  }
  
   	for (int i = 0; i < idUser; i++)
	{
			data_UsersSort = user_semelhantes[i];
			Empilha(data_UsersSort,P2);
	}
    	
  printf("\n\n");

	printf("===================================Pilha com Usuarios Semelhantes Ordenados==================================\n");	
	ImprimePilha(P2);
	printf("==============================================================================================================\n"); 
	
	Item *movies_sorted;
	movies_sorted = (Item*)malloc(idFilme*sizeof(Item));

	//int best_movies[18];
	int k,l;
	int *best_movies;
	best_movies = (int*)malloc(18*sizeof(int));
	int *best_movies_rating;
	best_movies_rating = (int*)malloc(18*sizeof(int));
	printf("\n==============Melhores Filmes User1========================\n");
	for (int i = 0; i < idUser; i++)
	{
		
		
		for (int j = 0; j < idFilme; j++)
		{

			if(mat[i][j]!=0 && Notas_User[j]==0 && user_semelhantes[i]==mat[i][j] && movies_sorted[j].filme!=j)
			{
				dado.filme = j;
				dado.nota = mat[i][j];
				movies_sorted[j] = dado;
				best_movies[k] = dado.filme;
				k++;
				best_movies_rating[l] = dado.nota;
				l++;
				Listar(dado,L);
				printf("Filme %d Nota: %d \n",movies_sorted[j].filme,movies_sorted[j].nota);	
			}
	
		}
	}

	printf("=======================================================\n\n");
	int aux3,aux4;
  for (int j = 1; j < idFilme ;j++) {
    for (int i = 0; i < idFilme - 1; i++) {
      if (movies_sorted[i].nota > movies_sorted[i + 1].nota) {
        aux3 = movies_sorted[i].nota;
        movies_sorted[i].nota = movies_sorted[i + 1].nota;
        movies_sorted[i + 1].nota = aux3;
        aux4 = movies_sorted[i].filme;
        movies_sorted[i].filme = movies_sorted[i+1].filme;
        movies_sorted[i+1].filme= aux4;
      }
    }
 
  }

	/*printf("\n==============Melhores Filmes Ordenados===============\n");
	for(int i=0;i<idFilme;i++)
	{
		
		if(movies_sorted[i].filme && movies_sorted[i].nota !=0)
		printf("Filme %d Nota: %d\n",movies_sorted[i].filme,movies_sorted[i].nota);	
		

	}
	printf("========================================================\n");*/
	
	printf("\n");
    fclose(file);

  // printf("========================================================\n\n");
  
   ////////////ETAPAS 4 e 5 a partir da linha 418
    HashTable h;
    HashTable *hash_pointer;
    Data d;
	int M = 9;//quantos filmes o user1 viu
	
	char linha2[100];
	const char delimitador2[] = ",";
	char *token2;
	int user_moviesid;
	Initialize(&h, M);
	printf("\n========================Inicializando a Tabela Hash===================\n\n");
	Imprime(&h);
	int iterator=0;
	// abrindo arquivo do user1
	FILE *file2;
	file2 = fopen(address, "r");
	
	//c é o contador que representa o maximo de filmes que o user1 viu que peguei quando estava tokenizando o user 1
	//na linha 234
	int *user_movies;
	user_movies= (int*)malloc(c*sizeof(int));

 // pegar o vetor com os ids dos filmes do usuario de entrada para servir como chave da tabela hash
	while(fgets(linha2,sizeof(linha2),file2) != NULL)
	{
		token2 = strtok(linha2, delimitador2);
		user_moviesid= atoi(token2);
		
		token2 = strtok(NULL,delimitador2);
			user_movies[iterator] = user_moviesid;
			iterator++;
	}
	
	fclose(file2);
	// preenchendo a tabela hash
	for(int i=0;i<M;i++)
	{
		Insert(&h,user_movies[i],best_movies);
	}
	

	printf("\n\t========================Tabela Hash========================\n\n");
	Imprime(&h);
	// ler e pegar os filmes do usuario 2 para depois comparar com as chaves da hash(filmes do user 1)
	//declarando algumas variaveis, definindo alguns vetores dinamicos e o token
	char linha3[100];
	const char delimitador3[] = ",";
	char *token3;
	int user2_moviesid;
	int iterator2=0;
	

	FILE *file3;
	file3 = fopen("users/user2.csv", "r");
	int *user2_movies;
	user2_movies= (int*)malloc(69*sizeof(int));
	int *user2_movies_rates;
	user2_movies_rates= (int*)malloc(idFilme*sizeof(int));
	int cont_aux=0;

	for (int i = 0; i < idFilme; ++i)
	{
		user2_movies_rates[i]=0;
	}
	// 69 é o tamanho do vetor user2_movies , pois o user2 viu 69 filmes
	while(fgets(linha3,sizeof(linha3),file3) != NULL)
	{
		token3 = strtok(linha3, delimitador3);
		user2_moviesid= atoi(token3);
		
		token3 = strtok(NULL,delimitador3);
		

		user2_movies[iterator2] = user2_moviesid;
		user2_movies_rates[user2_moviesid]=atoi(token3);
		iterator2++;
			
	}
	printf("\n");
	//ImprimeChaves(&h);
	//printf("\n");

	
	printf("\n");
	//declarando variaveis 
	int cont2;
	int user2_semelhantes[idUser];

	printf("\n\n=============================Usuarios Semelhantes do USER2=================================================\n\n");
	for(int i=0;i<idUser;i++)
	{
		cont=0;
		for(int j=0;j<idFilme;j++)
		{
			// se for diferente de 0 a nota dada pelo usuario de entrada e o da matriz quer dizer que ambos viram
			if(mat[i][j]!=0 && user2_movies_rates[j]!=0)
			{
				cont2++;
			}
		}
		//filtrando baseado em uma constante arbitraria os usuarios semelhantes que viram pelo menos 1  filme igual ao
		//usuario de entrada
		if(cont2>=1)
		{
			user2_semelhantes[i]=i;
			printf("Usuario %d avaliou o mesmo filme %d vezes\n",user2_semelhantes[i],cont2);
		}
	}
	//declaração de variáveis
	int cont_best_movies=0;
	int *best_movies_user2;
	best_movies_user2= (int*)malloc(idFilme*sizeof(int));
	printf("\n========================Melhores Filmes USER2========================\n");
	for (int i = 0; i < idUser; i++)
	{
		for (int j = 0; j < idFilme; j++)
		{

			if(mat[i][j]!=0 && user2_movies_rates[j]==0 && user2_semelhantes[i]==mat[i][j] && best_movies_user2[j]!=j)
			{
				best_movies_user2[cont_best_movies] = j;

				cont_best_movies++;
			}
	
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if(best_movies_user2[i]!=0)
		printf("Filme %d\n",best_movies_user2[i]);	
	}



	Initialize(&h,M);
	int cont3=0;

	for (int i = 0; i < iterator2 ; ++i)
	{
		for (int j = 0; j < 10; j++)
		{
			if(user2_movies[i] != user_movies[j]){
			//como as minhas chaves sao os id dos filmes do user1 posso comparar com o vetor que tem
			//esses filmes , se o filme que o usuario 2 viu mas não esta na hash eu vou e somo 1 no contador cont3
			// e se esse contador tiver valor de pelo menos 1 ,quer dizer que pelo menos 1 filme do user2 nao esta na hash
			// e assim eu atualizo a tabela hash com os melhores filmes para o usuario 2
				cont3++;	
			}
			if(cont3>=1)
			{
				Insert(&h,user_movies[j],best_movies_user2);
			}
		}
	}
	printf("\n\t========================Tabela Hash Atualizada========================\n\n");
	//como eu mando apenas 10 dos melhores filmes no meu insert , dessa vez como tive apenas 9 melhores filmes
	// o ultimo ficara com valor 0 visto que apenas 9 foram achados para o usuario 2	
	Imprime(&h);
				
				

	
	
	printf("\n");
	
}

#endif





























