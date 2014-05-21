#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <instrucao.h>

#define NUM_TOTAL_PECAS 6

typedef struct peca *pPeca;

typedef struct peca
{
	int id;
	int numEsq;
	int numDir;
	pPeca prox;
	
}Peca;

void mostraPecas(pPeca pecas){
	pPeca mostrar = pecas;
	while (mostrar -> prox != NULL)
	{
		
		printf("PECA ID %d= | %d | %d |\n",mostrar-> id, mostrar -> numEsq, mostrar -> numDir);
		mostrar = mostrar -> prox; 
	}
	if(mostrar -> prox == NULL){
		printf("PECA ID %d= | %d | %d |\n",mostrar-> id, mostrar -> numEsq, mostrar -> numDir);
	}
}
pPeca geraPecas(){
	
	int i = 0;
	int j = 0;
	int id = 0;
	pPeca auxHead = NULL;
	pPeca auxCurr = NULL; // Peca currente
	pPeca auxUltPeca = NULL; // Peca currente
	for (i = 0; i <= NUM_TOTAL_PECAS; i++)
	{
		
		for (j = 0; j <= i; j++)
		{
			
			auxCurr = (pPeca)malloc(sizeof(Peca));
			auxCurr -> id = id++;
			auxCurr -> numEsq = i;
			auxCurr -> numDir = j;
			auxCurr -> prox = NULL;
			
			if(auxHead == NULL){
				auxHead = auxCurr;
				auxUltPeca = auxHead;
			}else{
				
				while(auxUltPeca -> prox != NULL ){
					auxUltPeca = auxUltPeca -> prox;
				}
				
				auxCurr -> prox = auxUltPeca -> prox; 
				auxUltPeca -> prox = auxCurr;
		
			}
		}
	}
	return auxHead;
}
int main(){
	
	pPeca pecas = geraPecas(pecas); // inicio da lista
	
	mostraPecas(pecas);
	
	
	exit(0);
}
