#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "somino.h"

void mostra_Tab(pPecas p)     //Recebe como argumento o ponteiro da lista.

{
	while (p != NULL) {
		printf("|%d|%d|", p->numEsq, p->numDir);
		p = p->prox;    //SALTA PARA O PROXIMO ELEMENTO DA LISTA
	}
}

void preenche(pPecas p, int valEsq, int ValDir, int idPeca){
	p->id = idPeca;
	p->numEsq = valEsq;
	p->numDir = ValDir;
	p->distribuida = 1;
	p->ant = NULL;
	p->prox = NULL;
}

pPecas jogarEsq (pPecas Tab, int valEsq, int valDir, int idPeca){

	pPecas nova;
	if ((nova = malloc(sizeof(Pecas))) == NULL)
		printf("Erro na alocacao de memoria\n");
	else {
		preenche(nova, valEsq, valDir, idPeca);
		nova->prox = Tab;
		Tab = nova;
	}
	return Tab;
}

int main(){
	int i, j;
	int nJogadores;
	int idPeca;
	unsigned int randvalue;
	Pecas Molho[28];
	pPecas Tabuleiro = NULL;
	Jogadores *Jogador = NULL;
	srand((unsigned)time(NULL));
	int valEsq, valDir;
	int index;
	
	cria_molho(Molho);
	
	/* obter o numero de jogadores e alocar memória para eles */
	printf("Introduza o numero de jogadores: ");
	scanf("%d", &nJogadores);
	fflush(stdin);

	Jogador = malloc(nJogadores * sizeof(Jogadores));

	// Obter / Inicializar a informação dos jogadores e suas peças
	
	for (i = 0; i < nJogadores ; i++){
		Jogador[i].id = i + 1;
		printf("Introduza o nome do jogador %d: ", i);
		scanf("%s", Jogador[i].nome);
		fflush(stdin);
		printf("\n");
	}
	
	// inicializar as pecas dos jogadores
	for (i = 0; i < nJogadores ; i++) {
		for (j = 0; j <= 27 ; j++){
			Jogador[i].Peca[j].id = -1;
			Jogador[i].Peca[j].numEsq = -1;
			Jogador[i].Peca[j].numDir = -1;
			Jogador[i].Peca[j].distribuida = 1;
		}
	}
	// distribuir as pecas pelos jogadores
	for (i = 0; i < nJogadores; i++){
		for (j = 0; j < 7; j++){
			do {
				randvalue = rand() % 28;
			} while (Molho[randvalue].distribuida != 0);
			Jogador[i].Peca[j].id = Molho[randvalue].id;
			Jogador[i].Peca[j].numEsq = Molho[randvalue].numEsq;
			Jogador[i].Peca[j].numDir = Molho[randvalue].numDir;
			Jogador[i].Peca[j].distribuida = 0;
			Molho[randvalue].distribuida = 1;
		}
	}
	// imprimir lista de pecas dos jogadores
	for (i = 0; i < nJogadores; i++){
		printf("\nJogador %d:\n\nA tua Lista de pecas:\n", Jogador[i].id);
		for (j = 0; j <= 27 ; j++){
			if (Jogador[i].Peca[j].distribuida == 0)
				printf("Numero %2d: |%d|%d|\n", Jogador[i].Peca[j].id, Jogador[i].Peca[j].numEsq, Jogador[i].Peca[j].numDir);
		}
	}
	
	
	
	// procurar nas pecas do jogador o indice da peca com o idPeca
	for (i = 0; i < nJogadores; i++){
		printf("Jogador %d:\nIntroduz o id da peca que queres jogar: ", i + 1);
		scanf("%d", &idPeca);
		for (j = 0; j <= 27; j++){
			if (Jogador[i].Peca[j].id == idPeca)
				index = j;
		}
		valEsq = Jogador[i].Peca[index].numEsq;
		valDir = Jogador[i].Peca[index].numDir;
		if (Tabuleiro == NULL)
		Tabuleiro = jogarEsq(Tabuleiro, valEsq, valDir, idPeca);
	}
	mostra_Tab(Tabuleiro);
	free(Jogador);
	exit(EXIT_SUCCESS);
}