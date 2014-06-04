#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <signal.h>
#include "instrucao.h"
#include "somino.h"



#define PIPE_NAME "servidor"
#define BUFFER_SIZE 200
#define NUM_MAX_JOGADORES 4

int numPlayers = 0;
int proxJogador = 0;
pid_t jogadorActual;
Jogadores players[NUM_MAX_JOGADORES];
int pipe_fd;
int hasGameStarted = 1; // ainda não há jogo

void sair(){
	printf("BYE BYE!!!\n");	
	sleep(1);	
	close(pipe_fd);
	unlink(PIPE_NAME); //Remove o PIPE
	exit(0);
}

void clientCOM(char *clientPipeName,  char msg[BUFFER_SIZE +1]){
	// cliente <- servidor
		char buffer[BUFFER_SIZE +1];
		//InsServerStruct send = insServerStruct;
		int pipe_fd;
		int open_mode = O_WRONLY;// O pipe só lê ou so escreve nunca os dois ao mesmo tempo
		int resp;
		
		if ((pipe_fd = open(clientPipeName,open_mode) )== -1)
		{
			perror("ERRO ao abrir o fifo\n");
			exit(-1);
		}
		strcpy(buffer, msg);
		if ((resp = write(pipe_fd,buffer,strlen(buffer))) == -1)//Tenta escrever no PIPE
		{
			perror("ERRO ao escrever no fifo\n");
			exit(-1);
		}
		
		buffer[strlen(msg)]='\0';		
		sleep(1);
		close(pipe_fd);
		return;
};

char* gereInstrucoes(InsSC ins){
		fflush(stdout);
		int i = 0;
		char *cmd[4];
		
		char* token;
		char* saveptr;
		char clientPipeName[20];
		token = strtok_r(ins.instrucacao, " ", & saveptr);
		if(token == NULL){
			printf("null");
			cmd[i++] = ins.instrucacao;
		}else{
			
			while( token != NULL){
				
				
				
				//strcat(commandoValidado, "\0");
				//fflush(stdout);
				
				cmd[i++] =  token;
				//strcat(cmd[i], "\0"); 
				token = strtok_r(NULL, " ", & saveptr);
			}
		}	
		
		
		
		if(i>0){

			//Comandos reconhecidos pelo cliente
			char *resp;
			if(strstr(cmd[0],"exit") != NULL){
				resp = ("recebi o comando exit ");
				sair();
			}else if(strstr(cmd[0],"logout") != NULL){
				resp=("recebi o comando logout \n");
			}else if(strstr(cmd[0],"satus") != NULL){
				resp=("recebi o comando satus \n");
			}else if(strstr(cmd[0],"users") != NULL){
				resp=("recebi o comando users \n");
			}else if(strstr(cmd[0],"new") != NULL && hasGameStarted == 1){
				hasGameStarted = 0; 
				resp=("jogo vai comecar");
				jogadorActual = players[proxJogador].pid;
				//sprintf(clientPipeName,"%d",players[proxJogador].pid);
				//clientCOM(clientPipeName,resp);
				//proxJogador++;
				//resp="";
			}else if(strstr(cmd[0],"play") != NULL){
				if(hasGameStarted == 0){
					printf("jogador= %d\n",proxJogador);
					resp=("a jogar");
					printf("a jogar[%d]==[%d]cmd enviado\n",jogadorActual,ins.pid );
					if(ins.pid == jogadorActual){
						
						if( proxJogador < (numPlayers-1) ){
							proxJogador++;
						}else
						{
							proxJogador= 0;
						}
						
						kill(players[proxJogador].pid, SIGUSR1); // -> manda o sinal para o proximo jogador
						jogadorActual = players[proxJogador].pid; // o proximo jogador passa a ser o actual
						//sprintf(clientPipeName,"%d",players[proxJogador].pid);
						//clientCOM(clientPipeName,resp);
						
					
						resp="";
					}else{
						resp="Não é a tua vez";
					}
					
				}else{
					resp = "o Jogo ainda nao começou!";
				}
			}else if(strstr(cmd[0],"quit") != NULL){
				resp=("recebi o comando quit \n");
			}else{
				//printf("Comando não conhecido");
				return "";
			}
			return resp;
		}
		return "";
}

void serverStart(){
	InsSC ins;
	char clientPipeName[20];
	int open_mode = O_RDONLY;// O pipe só lê ou so escreve(O_WRONLY) nunca os dois ao mesmo tempo; O_NONBLOCK faz com que o PIPE nao fique ha espera de receber alguma coisa
	
	int resp;
	
	
//	char buffer[BUFFER_SIZE +1 ];
	if(access(PIPE_NAME,F_OK)){//Verifica se não existe este pipe, retorna valor "OK" de não existir
		
		if(mkfifo(PIPE_NAME,0777) == 0 ) { // Cria PIPE
			printf("O fifo foi criado\n");
		}else{
			perror("erro ao criar o fifo\n");
			exit(-1);		
		}
		
	}else{
		printf("PIPE já existe\n"); 
	}
	
	if ((pipe_fd = open(PIPE_NAME,open_mode) )== -1)
	{
		perror("ERRO ao abrir o fifo\n");
		exit(-1);
	}
	
	
	sleep(1);
	
	while(1)
	{
		if ((resp = read(pipe_fd,&ins,sizeof(InsSC))) == -1)//Tenta Ler do PIPE
		{
			perror("ERRO ao ler do fifo\n");
			exit(-1);
		}
		
		
			
		//if(resp == 0) break;
		
		if(ins.isServer == 0){
			
				printf("O Admin mandou me desligar :'('\n");
				sair();
		}
		
		
		if(resp > 0){
			
			int i = 0;
			int j = 0;
			int jaExiste = 1;
			for(i = 0; i < numPlayers +1 ; i++){
				
				if(players[i].pid == -1){
					players[i].pid = ins.pid;
					if(numPlayers < NUM_MAX_JOGADORES){
						numPlayers++;
					}
					printf("numPlayers=%d\n",numPlayers);
					break;
				}
				else{
					for(j=0; j < numPlayers ;j++){
						if(players[j].pid == ins.pid){
							jaExiste = 0;
							break;
						}
					}
					if(jaExiste == 0) break;
				}
			}
		
			char *cmd;
			sprintf(clientPipeName,"%d",ins.pid);	
			//printf("Foram lidos %d bytes sou o Cliente %s \n",resp,buffer);
			if(strcmp(ins.instrucacao,"") == 0){
				//sair();
				//exit(0);

				printf("Foram lidos %d bytes sou o Cliente %s \n",resp,clientPipeName);
				//sleep(1);
				clientCOM(clientPipeName,"Cliente conectado!");

			}else if(strstr((cmd = gereInstrucoes(ins)),"") != 0){
				clientCOM(clientPipeName,cmd);
				//sair();
			}
			
			
		}
		
	}
	
	close(pipe_fd);
	unlink(PIPE_NAME); //Remove o PIPE
}


void ProcSignal(int signal){
	sair();
}


int secondServer(){
	if (!access(PIPE_NAME,F_OK))
	{
		int tempPipe_fd;
		if ((tempPipe_fd = open(PIPE_NAME,O_WRONLY) )== -1)
		{
			printf("ERROO\n");
		}else{
			//char command[BUFFER_SIZE +1 ];
			int resp;
			InsSC ins; // Instrucao
			ins.pid = getpid();
			ins.isServer = 0; //  é servidor
			
			printf("insira Commando:\n");
			scanf("%s",ins.instrucacao);
			//sprintf(command,"%d",getpid());
			if ((resp = write(tempPipe_fd,&ins,sizeof(InsSC))) == -1)//Tenta escrever no PIPE
			{
				perror("ERRO ao escrever no fifo\n");
				return 1;
			}
			sleep(1);
			close(pipe_fd);
			
		}
		
		return 0;
	}
		
	return -1;	
}

int main(){

	//inicializa array de jogadores
	int j;
	for(j=0; j < NUM_MAX_JOGADORES ;j++){
		players[j].pid = -1;
	}
	
	pid_t pid;
	
	//Caso ja exista um servidor, executa outro servidor momentaneamente
	//Ou seja apos enviar um comando ele sair
	if(secondServer() == 0){
		exit(0);
	}
	
	if(signal(SIGUSR1, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	if(signal(SIGUSR2, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	
	//algoritmo para correr o servidor em background!
	switch((pid=fork())){
		case -1:
			/* Erro no fork */
			perror("[Erro] Fork...");
			exit(1);
		case 0:
			/* Processo filho */
			printf("[filho] Sou o filho com PID=%d.\n",getpid());
			serverStart();
			//exit(0);
		default:
			/* Processo pai */
			printf("[Pai] Sou o processo pai e gerei o filho com PID=%d.\n",pid);
			sleep(1);
			printf("[Pai] Adeus!\n");
			exit(0);
		}
		
	
	
	exit(0);
}
