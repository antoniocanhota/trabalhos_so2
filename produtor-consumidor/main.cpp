//============================================================================
// Nome: Produtor-Consumidor
// Autores: Antonio Canhota, Renato Farias, Rennan Oliveira
// Descrição: Programa que implementa o problema clássico de programação
//   paralela, Produtor-Consumidor.
//============================================================================

//============================================================================
// INCLUDES
//============================================================================

#include <cstdlib>
#include <pthread.h>
#include <queue>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#include "rfUtil.h"

using namespace std;

//============================================================================
// DEFINES
//============================================================================

// Número máximo de threads que podem ser criadas para produtores ou consumidores.
#define MAX_THREADS 16

// Número de itens que podem ser inseridos no buffer.
#define BUFFER_SIZE 8

//============================================================================
// STRUCTS
//============================================================================

// Estrutura que contém parâmetros individuais das threads.
// Por enquanto, só possui um id.
typedef struct thread_param {
	int my_id;
} thread_param;

//============================================================================
// VARIÁVEIS GLOBAIS
//============================================================================

// Mutex (semáforo binário)
pthread_mutex_t mutex;

// Semáforos
sem_t empty, full;

// Buffer (fila circular)
queue<int> buffer;

//============================================================================
// FUNÇÕES
//============================================================================

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

		//============================================================================
		// Seção crítica 
		//============================================================================
		// Produzir um item e inserir no buffer...
		int item = p->my_id;
		buffer.push( item );
		//============================================================================

		// Agora tem mais um lugar ocupado no buffer
		sem_post( &full );

		// Verificar e imprimir os valores dos semáforos
		sem_getvalue( &empty, &val_empty );
		sem_getvalue( &full, &val_full );
		printf( "Eu sou o produtor %i!\tempty: %i\tfull: %i\n", p->my_id, val_empty, val_full );

		// Liberar a seção crítica para outras threads
		pthread_mutex_unlock( &mutex );

		// Um pequeno delay para podermos ver o andamento das mensagens...
		sleep( my_rand( 5000 ) / 1000 );
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

		//============================================================================
		// Seção crítica
		//============================================================================
		// Consumir um item do buffer...
		int elem = buffer.front();
		buffer.pop();
		//============================================================================

		// Agora tem mais um lugar vazio no buffer
		sem_post( &empty );

		// Verificar e imprimir os valores dos semáforos
		sem_getvalue( &empty, &val_empty );
		sem_getvalue( &full, &val_full );
		printf( "Eu sou o consumidor %i!\tempty: %i\tfull: %i\n", p->my_id, val_empty, val_full );

		// Liberar a seção crítica para outras threads
		pthread_mutex_unlock( &mutex );

		// Um pequeno delay para podermos ver o andamento das mensagens...
		sleep( my_rand( 5000 ) / 1000 );
	}

	return NULL;
}

//============================================================================
// MAIN
//============================================================================

// Função principal.
int main( int argc, char* argv[] )
{
	//----------------------------------------------------------------------------
	// Declaração de variáveis
	//----------------------------------------------------------------------------

	// O número default de produtores e consumidores é 1
	int numero_de_produtores   = 1;
	int numero_de_consumidores = 1;

	// Ponteiros para as threads
	pthread_t* produtores;
	pthread_t* consumidores;

	// Ponteiros para parâmetros das threads
	thread_param* params_prod;
	thread_param* params_cons;

	//----------------------------------------------------------------------------
	// Leitura de parâmetros de entrada da linha de comando
	//----------------------------------------------------------------------------

	// Se tiver mais de um argumento (além do próprio programa), o usuário está querendo
	// especificar o número de produtores e consumidores.
	if( argc > 1 )
	{
		// O usuário precisa especificar quantos produtores e consumidores quer, ou seja,
		// além do programa são mais dois parâmetros de entrada.
		if( argc == 3 )
		{
			numero_de_produtores = atoi( argv[1] );
			if( ( numero_de_produtores < 1 ) || ( numero_de_produtores > MAX_THREADS ) ) {
				printf( "O número de produtores deve estar entre 1 e %d.\n", MAX_THREADS );
				return -1;
			}

			numero_de_consumidores = atoi( argv[2] );
			if( ( numero_de_consumidores < 1 ) || ( numero_de_consumidores > MAX_THREADS ) ) {
				printf( "O número de consumidores deve estar entre 1 e %d.\n", MAX_THREADS );
				return -1;
			}
		}
		else
		{
			printf( "\nModos de executar o programa:\n\n" );
			printf( "[nome do programa]\n-Executará com 1 produtor e 1 consumidor.\n\n" );
			printf( "[nome do programa] [num de produtores] [num de consumidores]\n-Permite especificar quantos produtores e consumidores serão criados.\n\n" );
			return -1;
		}
	}

	//----------------------------------------------------------------------------
	// Alocação de memória
	//----------------------------------------------------------------------------

	// Alocar memória para as threads
	produtores = (pthread_t*)malloc( numero_de_produtores * sizeof( *produtores ) );
	if( produtores == NULL ) {
		printf( "Erro alocando memória para produtores.\n" );
		return -1;
	}

	consumidores = (pthread_t*)malloc( numero_de_consumidores * sizeof( *consumidores ) );
	if( consumidores == NULL ) {
		printf( "Erro alocando memória para consumidores.\n" );
		return -1;
	}

	// Alocar memória para os parâmetros das threads
	params_prod = (thread_param*)malloc( numero_de_produtores * sizeof( thread_param ) );
	if( params_prod == NULL ) {
		printf( "Erro alocando memória para parâmetros dos produtores.\n" );
		return -1;
	}

	params_cons = (thread_param*)malloc( numero_de_consumidores * sizeof( thread_param ) );
	if( params_cons == NULL ) {
		printf( "Erro alocando memória para parâmetros dos consumidores.\n" );
		return -1;
	}

	//----------------------------------------------------------------------------
	// Inicialização
	//----------------------------------------------------------------------------

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

	//----------------------------------------------------------------------------
	// Criação das threads
	//----------------------------------------------------------------------------

	// Criar os produtores
	printf( "Criando %i produtor(es)!\n", numero_de_produtores );
	for( int i = 0; i < numero_de_produtores; i++ )
	{
		params_prod[i].my_id = i;

		if( pthread_create( &produtores[i], NULL, produtor, (void *)(params_prod + i) ) ) {
			printf( "Erro criando produtor de id %i.\n", i );
			return -1;
		}
	}

	// Criar os consumidores
	printf( "Criando %i consumidor(es)!\n", numero_de_consumidores );
	for( int i = 0; i < numero_de_consumidores; i++ )
	{
		params_cons[i].my_id = i;

		if( pthread_create( &consumidores[i], NULL, consumidor, (void *)(params_cons + i) ) ) {
			printf( "Erro criando consumidor de id %i.\n", i );
			return -1;
		}
	}

	//----------------------------------------------------------------------------
	// Sincronização das threads
	//----------------------------------------------------------------------------

	// Sincronizar os produtores
	for( int i = 0; i < numero_de_produtores; i++ )
	{
		if( pthread_join( produtores[i], NULL ) ) {
			printf( "Erro no join do produtor de id %i\n", i );
			return -1;
		}
	}

	// Sincronizar os consumidores
	for( int i = 0; i < numero_de_consumidores; i++ )
	{
		if( pthread_join( produtores[i], NULL ) ) {
			printf( "Erro no join do consumidor de id %i\n", i );
			return -1;
		}
	}

	//
	// Teoricamente, o programa nunca passará desse ponto, já que os produtores e
	// consumidores ficam em loop infinito.
	//

	//----------------------------------------------------------------------------
	// Limpeza
	//----------------------------------------------------------------------------

	// Liberar memória alocada
	free( produtores );
	free( consumidores );
	free( params_prod );
	free( params_cons );

	// Destruir o mutex
	pthread_mutex_destroy( &mutex );

	// Destruir os semáforos
	sem_destroy( &empty );
	sem_destroy( &full );

	// Acabou
	printf( "Todas as threads terminaram.\n" );
	return 0;
}
