#ifndef SOMINO_H
#define SOMINO_H

#include <fcntl.h>

typedef struct peca  Pecas, *pPecas;

struct peca {
	int id;							// numero da peça
	int numEsq;						// valor à esquerda da peça
	int numDir;						// valor à direita da peça
	int distribuida;				// já foi distribuida a um jogador / já foi jogada???
	pPecas prox;
	pPecas ant;
};

typedef struct jogador Jogadores;

struct jogador {
	pid_t pid;	// id de jogador (pid)
	int isPlaying;		// se tiver a jogar esta a 0				
	char nome[50];					// nome do jogador
	Pecas Peca[28];					// Peças do jogador
};

// Prototipos das funções
void cria_molho(Pecas *molho);

#endif /* SOMINO_H */
