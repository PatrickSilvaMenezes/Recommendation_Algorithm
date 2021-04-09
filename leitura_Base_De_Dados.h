#ifndef _LEITURABASEDEDADOS_
#define _LEITURABASEDEDADOS_
//include de bibliotecas
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
//definindo o id maximo
#define idUser 30
#define idFilme 494


void read_file(FILE *file, char *address){
	char linha[100];

	file = fopen(address,"r");

	if (file == NULL)
		printf("Erro! Não foi possível abrir o arquivo!");

	while(fgets(linha, 100, file) !=  NULL){
  		printf("%s", linha);
	}

	fclose(file);
}
int get_IdUserMax(FILE *file,char *address){

	char linha[100];
	file = fopen(address,"r");
	const char delimitador[] = ",";
    char *token;
    int max=0,line=0,i=0;

	if (file == NULL)
		printf("Erro! Não foi possível abrir o arquivo!");
	while(fgets(linha,sizeof(linha),file) != NULL)
	{
		token = strtok(linha, delimitador);
		
		line = strtol(token,NULL,10);
		token = strtok(NULL,delimitador);
		if(max<line)
		{
			max = line;
		}
		
	}
	return  max;
}
int** register_file(FILE *file, char *address){
    
    char linha[100];
    const char delimitador[] = ",";
    char *token;
  	int line=0,column=0;
    int **mat_netflix;
    file = fopen(address,"r");
    

	if (file == NULL)
		printf("Erro! Não foi possível abrir o arquivo!");
	//preparando minha matriz, zerando elemento por elemento para evitar lixo eletronico
    mat_netflix = (int **)malloc(idUser * sizeof(int*));
	for(int i=0;i < idUser;i++)
    {	
    	mat_netflix[i] = (int*)malloc(idFilme * sizeof(int));
    	for (int j = 0; j < idFilme; j++)
    	{
    		mat_netflix[i][j]=0;
    	}
    }

    while(fgets(linha,sizeof(linha),file) != NULL)
	{
		token = strtok(linha, delimitador);
		
		if(atoi(token)>=0 && atoi(token)<idUser)
		{
			line = atoi(token);
			token = strtok(NULL,delimitador);//parei quando achei o primeiro delimitador, após isso poderei seguir para o proximo elemento
		}
		if(atoi(token)>=0 && atoi(token)<idFilme)
		{
			column = atoi(token);
			token = strtok(NULL,delimitador);//depois do id do filme irei setar a ultima informaçao(nota) apos o delimitador
		}

		mat_netflix[line][column] = atoi(token); 
		
    }
  
    printf("\n");
    fclose(file);
   	
   return mat_netflix;  
}
void imprime_BaseNetflix(int **mat)
{
	for(int i=0; i<idUser; i++)
	{
		for(int j=0; j<idFilme; j++)
		{
			printf("%d\t", mat[i][j]);
		}
		printf("\n");
	}
}
#endif

 
