//============================================================================
// Name : processos
// Author : Antonio Canhota, Renato Farias, Rennan Oliveira
// Version : 1.0
// Copyright : 
// Description : Programa que cria 5 processos usando o comando fork.
//============================================================================

#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{  		
	pid_t pID = -1;
	printf( "Processo pai criado. Criando processo(s) filho em 3 segundos...\n" );
	sleep(3);
	
	for( int i = 0; i < 5; i++ )
	{
		if( pID != 0 )
		{
			pID = fork();
			if (pID < 0)	//Tratamento de erro
			{
				printf("ERRO AO CRIAR PROCESSO FILHO\n");
			}
		}
	}

	if( pID == 0 ) // Processo(s) filho
	{
		printf( "Processo filho sendo executado (Analisador de disco)...\n" );
		execvp( "baobab", NULL );
	}
	else // Processo pai
	{
		printf( "Processo pai sendo executado (Firefox, que depende do filho executar)...\n" );
		execvp( "firefox", NULL );
	}

	return 0;
}
