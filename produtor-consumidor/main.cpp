//============================================================================
// Name : Produtor-Consumidor
// Author : Antonio Canhota, Renato Farias, Rennan Oliveira
// Version : 1.0
// Copyright : 
// Description : Programa que implementa o problema clássico de programação
//   paralela, Produtor-Consumidor.
//============================================================================

#include <cstdlib>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "rfUtil.h"

// Número máximo de threads que podem ser criadas.
#define MAX_THREADS 256

// Número de itens que podem ser inseridos no buffer.
#define BUFFER_SIZE 8

// Mutex (semáforo binário)
pthread_mutex_t mutex;

// Semáforos
sem_t empty, full;

// Estrutura que contém parâmetros individuais das threads.
// Por enquanto, só possui um id.
typedef struct thread_param {
	int my_id;
} thread_param;

// Função que os produtores executarão.
void* produtor( void* arg )
{
	int val_empty, val_full;
	thread_param* p = (thread_param*)arg;

	while( true )
	{
		// Esperar ter espaço para produzir
		sem_wait( &empty );

		// Exclusão mútua para entrar na seção crítica
		pthread_mutex_lock( &mutex );

		// Seção crítica

		// TODO: Produzir...

		// Agora tem mais um lugar ocupado no buffer
		sem_post( &full );

		// Verificar e imprimir os valores dos semáforos
		sem_getvalue( &empty, &val_empty );
		sem_getvalue( &full, &val_full );
		printf( "Eu sou o produtor %i!\tempty: %i\tfull: %i\n", p->my_id, val_empty, val_full );

		// Liberar a seção crítica para outras threads
		pthread_mutex_unlock( &mutex );

		// Um pequeno delay para podermos ver o andamento das mensagens...
		sleep( my_rand( 8000 ) / 1000 );
	}

	return NULL;
}

// Função que os consumidores executarão.
void* consumidor( void* arg )
{
	int val_empty, val_full;
	thread_param* p = (thread_param*)arg;

	while( true )
	{
		// Esperar ter algo para consumir
		sem_wait( &full );

		// Exclusão mútua para entrar na seção crítica
		pthread_mutex_lock( &mutex );

		// Seção crítica

		// TODO: Consumir...

		// Agora tem mais um lugar vazio no buffer
		sem_post( &empty );

		// Verificar e imprimir os valores dos semáforos
		sem_getvalue( &empty, &val_empty );
		sem_getvalue( &full, &val_full );
		printf( "Eu sou o consumidor %i!\tempty: %i\tfull: %i\n", p->my_id, val_empty, val_full );

		// Liberar a seção crítica para outras threads
		pthread_mutex_unlock( &mutex );

		// Um pequeno delay para podermos ver o andamento das mensagens...
		sleep( my_rand( 8000 ) / 1000 );
	}

	return NULL;
}

// Função principal.
int main( int argc, char* argv[] )
{
	// O número default de threads é 2
	int number_of_threads = 2;

	// Ponteiro para as threads
	pthread_t* pthreads;

	// Ponteiro para parâmetros das threads
	thread_param* params;

	// Ler o número de threads como parêmetro de entrada
	if( argc > 1 )
	{
		number_of_threads = atoi( argv[1] );

		if( ( number_of_threads < 2 ) || ( number_of_threads > MAX_THREADS ) ) {
			printf( "O número de threads deve estar entre 2 e %d.\n", MAX_THREADS );
			return -1;
		}
	}

	// Alocar memória para as threads
	pthreads = (pthread_t*)malloc( number_of_threads * sizeof( *pthreads ) );
	if( pthreads == NULL ) {
		printf( "Erro alocando memória para pthreads.\n" );
		return -1;
	}

	// Alocar memória para os parâmetros das threads
	params = (thread_param*)malloc( number_of_threads * sizeof( thread_param ) );
	if( params == NULL ) {
		printf( "Erro alocando memória para parâmetros.\n" );
		return -1;
	}

	// Inicializar o mutex
	if( pthread_mutex_init( &mutex, NULL ) ) {
		printf( "Erro inicializando mutex.\n" );
		return -1;
	}

	// Inicializar o semáforo 'empty' com o tamanho do buffer. O segundo argumento 0 significa
	// que o semáforo é compartilhado entre threads, e não processos.
	if( sem_init( &empty, 0, BUFFER_SIZE ) ) {
		printf( "Erro inicializando semáforo 'empty'.\n" );
		return -1;
	}

	// Inicializar o semáforo 'full' com 0
	if( sem_init( &full, 0, 0 ) ) {
		printf( "Erro inicializando semáforo 'full'.\n" );
		return -1;
	}

	// Inicializar o seed para números aleatórios.
	srand( get_clock_msec() );

	// Criar as threads
	for( int i = 0; i < number_of_threads; i++ )
	{
		params[i].my_id = i/2;

		// Criar metade das threads como produtores, metade como consumidores.
		// TODO: Permitir um número diferente de produtores e consumidores.
		if( i % 2 == 0 ) {
			if( pthread_create( &pthreads[i], NULL, produtor, (void *)(params + i) ) ) {
				printf( "Erro criando produtor de id %i (thread %i).\n", i/2, i );
				return -1;
			}
		}
		else {
			if( pthread_create( &pthreads[i], NULL, consumidor, (void *)(params + i) ) ) {
				printf( "Erro criando consumidor de id %i (thread %i).\n", i/2, i );
				return -1;
			}
		}
	}

	// Sincronizar as threads
	for( int i = 0; i < number_of_threads; i++ )
	{
        if( pthread_join( pthreads[i], NULL ) ) {
            printf( "Erro no join da thread de id %i\n", i );
            return -1;
        }
	}

	//
	// Teoricamente, o programa nunca passará desse ponto, já que os produtores e
	// consumidores ficam em loop infinito.
	//

	// Liberar memória alocada
	free( pthreads );
	free( params );

	// Destruir o mutex
	pthread_mutex_destroy( &mutex );

	// Destruir os semáforos
	sem_destroy( &empty );
	sem_destroy( &full );

	// Acabou
	printf( "Todas as threads terminaram.\n" );
	return 0;
}
