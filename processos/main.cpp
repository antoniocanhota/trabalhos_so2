//============================================================================
// Name : main.cpp
// Author : Antonio Canhota, Renato Farias, Rennan Oliveira
// Version : 1.0
// Copyright : 
// Description : Programa que cria 2 processos usando o comando fork 
//============================================================================

#include <unistd.h>
#include <stdio.h>

main()
{  		
	pid_t pID;
	printf("Processo pai criado. Criando processo filho em 3 segundos...\n");
	sleep(3);
        pID = fork();

	if (pID == 0)		//Processo filho
   	{     
		printf("Processo filho sendo executado (Gedit)...\n");
		execvp("gedit",NULL);                
	}
	else if (pID < 0)	//Tratamento de erro
        {
		printf("ERRO AO CRIAR PROCESSO FILHO\n");
	}
        else			//Processo pai  
        {
		printf("Processo pai sendo executado (Firefox, que depende do Gedit executar)...\n");		
       		execvp("firefox",NULL);		
        }
	return 0;
}
                
