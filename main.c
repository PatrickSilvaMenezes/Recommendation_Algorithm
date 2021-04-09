#include "leitura_Base_De_Dados.h"
#include "usuarios_semelhantes.h"


int main()
{
	int **mat_netflix;
	FILE *file;
	char *address = "DataBaseNetflix/netflix.csv";
	int a;
	mat_netflix = register_file(file,address);
	//imprime_BaseNetflix(mat_netflix);
	//read_file(file,address);

	
	FILE *file2;
	char *address2 = "users/user1.csv";
	
	Recomendation(file2,address2,mat_netflix);
	
	
	return 0;
}