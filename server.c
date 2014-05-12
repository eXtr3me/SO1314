#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <signal.h>



#define PIPE_NAME "servidor"
#define BUFFER_SIZE 200
int pipe_fd;
void sair(){
	printf("BYE BYE!!!\n");	
	close(pipe_fd);
	unlink(PIPE_NAME); //Remove o PIPE
}
void ProcSignal(int signal){
	sair();
}
void clientCOM(char *clientPipeName){
	// cliente <- servidor
		
		int pipe_fd;
		int open_mode = O_WRONLY;// O pipe só lê ou so escreve nunca os dois ao mesmo tempo
		int resp;
		char buffer[BUFFER_SIZE +1 ];
		
		if ((pipe_fd = open(clientPipeName,open_mode) )== -1)
		{
			perror("ERRO ao abrir o fifo\n");
			exit(-1);
		}
		
		sprintf(buffer,"[SERVIDOR]Ola tudo bem, sou SERVIDOR %d",getpid()); 
		
		if ((resp = write(pipe_fd,buffer,strlen(buffer))) == -1)//Tenta escrever no PIPE
		{
			perror("ERRO ao escrever no fifo\n");
			exit(-1);
		}
		
		buffer[resp]='\0';
		printf("Foram lidos %d bytes com o conteudo\n %s \n",resp,buffer);
		
		sleep(2);
		close(pipe_fd);
		return;
};

void serverStart(){
	
	int open_mode = O_RDONLY;// O pipe só lê ou so escreve(O_WRONLY) nunca os dois ao mesmo tempo; O_NONBLOCK faz com que o PIPE nao fique ha espera de receber alguma coisa
	
	int resp;
	char buffer[BUFFER_SIZE +1 ];
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
		if ((resp = read(pipe_fd,buffer,BUFFER_SIZE)) == -1)//Tenta Ler do PIPE
		{
			perror("ERRO ao ler do fifo\n");
			exit(-1);
		}
		
		//if(resp == 0) break;
		
		if(resp > 0){
			
			
			
			buffer[resp]='\0';
			
			
			
			//printf("Foram lidos %d bytes sou o Cliente %s \n",resp,buffer);
			if(strncmp(buffer,"close")== 0){
				sair();	
				exit(0);
			}else if(strncmp(buffer,"show")== 0){
				//TODO ver especificação par o comando Show		
			}else{
				
				sleep(1);
				clientCOM(buffer);
			}
			
		}
		
	}
	
	close(pipe_fd);
	unlink(PIPE_NAME); //Remove o PIPE
}

int secondServer(){
	if (!access(PIPE_NAME,F_OK))
	{
		int tempPipe_fd;
		if ((tempPipe_fd = open(PIPE_NAME,O_WRONLY) )== -1)
		{
			printf("ERROO\n");
		}else{
			char command[BUFFER_SIZE +1 ];
			int resp;
			printf("insira Commando:\n");
			scanf("%s",command);
			//sprintf(command,"%d",getpid());
			if ((resp = write(tempPipe_fd,command,strlen(command))) == -1)//Tenta escrever no PIPE
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
	pid_t pid;
	
	//Caso ja exista um servidor, executa momentaneamente
	if(secondServer() == 0){
		exit(0);
	}
	
	// nao esta a matar, mas ta bemmm :P
	if(signal(SIGUSR1, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	if(signal(SIGUSR2, ProcSignal)==SIG_ERR){
		perror("Erro sinal");
	}
	switch((pid=fork())){
		case -1:
			/* Erro no fork */
			perror("[Erro] Fork...");
			exit(1);
		case 0:
			/* Processo filho */
			printf("[filho] Sou o filho com PID=%d.\n",getpid());
			serverStart();
			exit(0);
		default:
			/* Processo pai */
			printf("[Pai] Sou o processo pai e gerei o filho com PID=%d.\n",pid);
			sleep(1);
			printf("[Pai] Adeus!\n");
			exit(0);
		}
		
	
	
	exit(0);
}
