//============================================================================
// Nome: processos
// Autores: Antonio Canhota, Renato Farias, Rennan Oliveira
// Descrição: Programa que cria N processos (sendo N especificável pela
//   linha de comando). O processo pai executa o firefox, enquanto os
//   processos filhos executam outros programas.
//============================================================================

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

// Número máximo de processos que podem ser criados.
#define MAX_PROCS 8

// Função principal.
int main( int argc, char* argv[] )
{
	// O número default de processos é 5
	int number_of_procs = 5;

	// Esta variável conterá o id do processo recentemente criado
	pid_t pID = -1;

	// Ler o número de processos como parâmetro de entrada
	if( argc > 1 )
	{
		number_of_procs = atoi( argv[1] );

		if( ( number_of_procs < 1 ) || ( number_of_procs > MAX_PROCS ) ) {
			printf( "O número de processos deve estar entre 1 e %d.\n", MAX_PROCS );
			return -1;
		}
	}

	printf( "Processo pai criado. Criando %i processo(s) filho(s) em 3 segundos...\n", number_of_procs );
	sleep(3);
	
	// Criar os processos filhos
	for( int i = 0; i < number_of_procs; i++ )
	{
		// O único que irá criar processos é o processo pai. Para os processos filhos,
		// fork() retorna 0, e para o pai, retorna o id do processo criado. Portanto,
		// o único que entrará nesse if será o processo pai. (Note que os processos
		// filhos executam a parte do if que vem depois do fork(), por serem cópias.)
		if( pID != 0 )
		{
			// Criar um processo filho que é uma cópia deste
			pID = fork();

			// Valor de retorno < 0 significa erro
			if( pID < 0 ) {
				printf( "ERRO AO CRIAR PROCESSO FILHO\n" );
				return -1;
			}
		}
	}

	if( pID == 0 ) // Processo(s) filho(s)
	{
		printf( "Processo filho sendo executado (Analisador de disco)...\n" );
		execvp( "baobab", NULL );
	}
	else // Processo pai
	{
		printf( "Processo pai sendo executado (Firefox, que depende do filho executar)...\n" );
		execvp( "firefox", NULL );;
	}

	return 0;
}
