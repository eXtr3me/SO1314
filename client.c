#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PIPE_NAME "servidor"
#define BUFFER_SIZE 200
int main(){
	// cliente -> servidor
	int pipe_fd;
	int open_mode = O_WRONLY;// O pipe só lê ou so escreve nunca os dois ao mesmo tempo
	int resp;
	char buffer[BUFFER_SIZE +1 ];
	char clientPipeName[BUFFER_SIZE +1 ];
	if ((pipe_fd = open(PIPE_NAME,open_mode) )== -1)
	{
		perror("ERRO ao abrir o fifo\n");
		exit(-1);
	}
	
	sprintf(buffer,"%d",getpid());
	sprintf(clientPipeName,"%d",getpid());
	if ((resp = write(pipe_fd,buffer,strlen(buffer))) == -1)//Tenta escrever no PIPE
	{
		perror("ERRO ao escrever no fifo\n");
		exit(-1);
	}
	
	buffer[resp]='\0';
	printf("Foram lidos %d bytes com o conteudo\n %s \n",resp,buffer);
	
	sleep(1);
	close(pipe_fd);
	
	// cliente <- servidor
	open_mode = O_RDONLY;// O pipe só lê ou so escreve(O_WRONLY) nunca os dois ao mesmo tempo; O_NONBLOCK faz com que o PIPE nao fique ha espera de receber alguma coisa
	
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
	
	if ((pipe_fd = open(clientPipeName,open_mode) )== -1)
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
		
		if(resp == 0) break;
		if(resp > 0){
			buffer[resp]='\0';
			printf("Foram lidos %d bytes com o conteudo\n %s \n",resp,buffer);
		}
		
	}
	
	
	close(pipe_fd);
	unlink(clientPipeName); //Remove o PIPE
	
	//unlink(PIPE_NAME); //Remove o PIPE. Não se deve ter dois program cliente/servidor a apagar o mesmo PIPE
	exit(0);
}
	
