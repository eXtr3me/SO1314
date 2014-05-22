#include <stdlib.h>
#include <stdio.h>
#include "somino.h"

void cria_molho(Pecas *molho){
	int i, j;
	int k = 0;
	int id = 0;
	for (i = 0; i <= 6; i++){
		for (j = 0; j <= i; j++){
			molho[k].id = id++;
			molho[k].numEsq = i;
			molho[k].numDir = j;
			molho[k].distribuida = 0;
			k++;
		}
	}
}
