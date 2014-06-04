#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "instrucao.h"
#include "somino.h"

#define PIPE_NAME "servidor"
#define BUFFER_SIZE 200
int cliente_pipe_fd;
int pipe_fd;
int sai = -1;
char clientPipeName[20];
InsSC ins; 

char* preparaComando(char* comand){
	char* token;
	char* saveptr;
	char* result = "";
	
	token = strtok_r(comand, " ", & saveptr);
	if(token == NULL){
		
		return comand;
	}
	while( token != NULL){
		
		strcat(token, "\0");
		
		//strcat(commandoValidado, "\0");
		//fflush(stdout);
			
		if(strcmp(result,"") == 0){
			result = token;
		 }
		 else{
			strcat(result, " ");
			strcat(result, token);
			strcat(result, "\0");
		 }
		token = strtok_r(NULL, " ", & saveptr);
	}
	return result;
}
void sair(){
	sleep(1);
	close(cliente_pipe_fd);
	unlink(clientPipeName);
	sai = 0;
}
void verifyServer(){
	if(access(PIPE_NAME,F_OK)){
		printf("O Servidor não se econtra iniciado\n");
		exit(-1);
	}
}
void ServerWriteCOM(int validaIns){
		// cliente -> servidor
	
		int open_mode = O_WRONLY;// O pipe só lê ou so escreve nunca os dois ao mesmo tempo
		int resp;
		
		if ((pipe_fd = open(PIPE_NAME,open_mode) )== -1)
		{
			perror("ERRO ao abrir o fifo\n");
			exit(-1);
		}
		if(validaIns == 0){
			//TODO VALIDA INSTRUCAO
			
			printf("\ninsira instrução\n");
			fflush(stdout);
			//scanf("%s",comand);
			fgets(ins.instrucacao,sizeof(ins.instrucacao),stdin);
			
			sprintf(ins.instrucacao,"%s",preparaComando(ins.instrucacao)); // ValidaComando

			if(strcmp(ins.instrucacao,"\0") == 0 )
			{
				printf("commando invalido\n");
			}
		}
		
		if ((resp = write(pipe_fd,&ins,sizeof(InsSC))) == -1)//Tenta escrever no PIPE
		{
			perror("ERRO ao escrever no fifo\n");
			exit(-1);
		}
		
		if(strncmp(ins.instrucacao,"exit",sizeof(TAM_STRING +1)) == 0){
			sair();
		}
		close(pipe_fd);
		
		
		return;
}
void ServerReadCOM(){
	char buffer[BUFFER_SIZE +1 ];
	int resp;

	int open_mode = O_RDONLY;// O pipe só lê ou so escreve(O_WRONLY) nunca os dois ao mesmo tempo; O_NONBLOCK faz com que o PIPE nao fique ha espera de receber alguma coisa

	if ((cliente_pipe_fd = open(clientPipeName,open_mode) )== -1)
	{
		perror("ERRO ao abrir o fifo\n");
		exit(-1);
	}
	
	while(1)
	{
		if ((resp = read(cliente_pipe_fd,buffer,BUFFER_SIZE)) == -1)//Tenta Ler do PIPE
		{
			perror("ERRO ao ler do fifo\n");
			exit(-1);
		}

		if(resp == 0) break;
		if(resp > 0){
			buffer[resp]='\0';
			printf("Foram lidos %d bytes com o conteudo\n %s \n",resp,buffer);
		}

	}

	close(cliente_pipe_fd);
	//unlink(pipeName);
}

void createClientPipe(){
	
	// cliente <- servidor
	
	if(access(clientPipeName,F_OK)){//Verifica se não existe este pipe, retorna valor "OK" de não existir
		
		if(mkfifo(clientPipeName,0777) == 0 ) { // Cria PIPE
			
				
				printf("O fifo foi criado\n");
			
		}else{
			perror("erro ao criar o fifo\n");
			exit(-1);		
		}
	}else{
		printf("PIPE já existe\n"); 
	}
	
}
void comunicacoes(){
	
	while(sai != 0){
		ServerReadCOM();
		ServerWriteCOM(0);	
	}
	exit(0);
}

void ProcSignal(int signal){
	verifyServer();//verificar se já esta o servidor a correr
	if(signal == SIGUSR1 || signal == SIGUSR2){
		close(pipe_fd);
		printf("[%d]]É a minha vez de jogar\n\n\n\n",ins.pid);
		ServerWriteCOM(0);
		comunicacoes();
	}
}
int main(){
	// test
	
	
	if(signal(SIGUSR1, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	if(signal(SIGUSR2, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	//main program
	ins.pid = getpid();
	ins.isServer = -1; // Não é servidor
	sprintf(ins.instrucacao,"%s",""); 
	sprintf(clientPipeName,"%d",ins.pid);
	
	
	
	verifyServer();//verificar se já esta o servidor a correr
	createClientPipe();
	ServerWriteCOM(-1);	
	comunicacoes();
	
	 //Remove o PIPE
	
	//unlink(PIPE_NAME); //Remove o PIPE. Não se deve ter dois program cliente/servidor a apagar o mesmo PIPE
	exit(0);
}
	
