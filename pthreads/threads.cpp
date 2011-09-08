//============================================================================
// Name : main.cpp
// Author : Antonio Canhota, Renato Farias, Rennan Oliveira
// Version : 1.0
// Copyright : 
// Description : Programa que cria N threads (sendo N especificável pela
//   linha de comando), cada uma com um id único. As threads imprimem seus
//   ids no console.
//============================================================================

#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

// Número máximo de threads que podem ser criadas.
#define MAX_THREADS 256

// Estrutura que contém parâmetros individuais das threads.
// Por enquanto, só possui um id.
typedef struct thread_param {
	int my_id;
} thread_param;

// Função que cada thread executará. Imprime seu id.
void* hello( void* arg )
{
	thread_param *p = (thread_param*)arg;
	printf( "Eu sou a thread %i!\n", p->my_id );
	return NULL;
}

// Função principal.
int main( int argc, char* argv[] )
{
	// O número default de threads é 2
	int number_of_threads = 2;

	// Ponteiro para as threads
	pthread_t *pthreads;

	// Ponteiro para parâmetros das threads
	thread_param *params;

	// Ler o número de threads como parêmetro de entrada
	if( argc > 1 )
	{
		number_of_threads = atoi( argv[1] );

		if( ( number_of_threads < 1 ) || ( number_of_threads > MAX_THREADS ) ) {
			printf( "O número de threads deve estar entre 1 e %d.\n", MAX_THREADS );
			exit(1);
		}
	}

	// Alocar memória para as threads
	pthreads = (pthread_t*)malloc( number_of_threads * sizeof( *pthreads ) );
	if( pthreads == NULL ) {
		printf( "Erro alocando memória para pthreads.\n" );
		exit(1);
	}

	// Alocar memória para os parâmetros das threads
	params = (thread_param*)malloc( number_of_threads * sizeof( thread_param ) );
	if( params == NULL ) {
		printf( "Erro alocando memória para parâmetros.\n" );
		exit(1);
	}

	// Criar as threads
	for( int i = 0; i < number_of_threads; i++ ) {
		params[i].my_id = i;
		pthread_create( &pthreads[i], NULL, hello, (void *)(params + i) );
	}

	// Sincronizar as threads
	for( int i = 0; i < number_of_threads; i++ ) {
		pthread_join( pthreads[i], NULL );
	}

	// Liberar memória alocada
	free( pthreads );
	free( params );

	// Acabou
	printf( "Todas as threads terminaram.\n" );
	return 0;
}
