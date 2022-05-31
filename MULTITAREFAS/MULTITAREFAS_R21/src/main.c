/**
 * \file
 *
 * \brief Exemplos diversos de tarefas e funcionalidades de um sistema operacional multitarefas.
 *
 */

/**
 * \mainpage Sistema operacional multitarefas
 *
 * \par Exemplso de tarefas
 *
 * Este arquivo contem exemplos diversos de tarefas e 
 * funcionalidades de um sistema operacional multitarefas.
 *
 *
 * \par Conteudo
 *
 * -# Inclui funcoes do sistema multitarefas (atraves de multitarefas.h)
 * -# Inicialização do processador e do sistema multitarefas
 * -# Criação de tarefas de demonstração
 *
 */

/*
 * Inclusao de arquivos de cabecalhos
 */
#include <asf.h>
#include "stdint.h"
#include "multitarefas.h"
#include <stdlib.h>

/*
 * Prototipos das tarefas
 */
void tarefa_1(void);	/* Produtor */
void tarefa_2(void);	/* Consumidor*/

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2			(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)
#define TAM_BUFFER			 5

/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];

semaforo_t cheio = {0, 0};
semaforo_t vazio = {TAM_BUFFER, 0};
semaforo_t mutex = {1, 0};

uint32_t buffer[TAM_BUFFER];

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	system_init();
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
	
	CriaTarefa(tarefa_1, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 2);	/* Produtor */
	
	CriaTarefa(tarefa_2, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 1);	/* Consumidor */
		
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);
	
	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

/* Tarefas produtora */
void tarefa_1(void)
{
	while(1){
		SemaforoAguarda(&vazio);
		SemaforoAguarda(&mutex);
		buffer[cheio.contador] = 1;
		SemaforoLibera(&mutex);
		SemaforoLibera(&cheio);
	}
}

void tarefa_2(void)
{
	while(1){
		SemaforoAguarda(&cheio);
		SemaforoAguarda(&mutex);
		buffer[cheio.contador] = 0;
		SemaforoLibera(&mutex);
		SemaforoLibera(&vazio);
	}
}