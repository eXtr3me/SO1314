#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define TAM_STRING 200
 
typedef struct instruncaoStruct InsSC;
struct instruncaoStruct{
	pid_t pid;
	int isServer;
	char instrucacao[TAM_STRING + 1 ];
};

 
typedef struct instruncaoServerStruct InsServerStruct;
struct instruncaoServerStruct{
	int isYourTurn;
	char* msg;
};

