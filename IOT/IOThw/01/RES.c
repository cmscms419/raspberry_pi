#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void reverse(char* rev, char* message, int mes_len);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i = 0;
	
	char *rev = (char*)malloc(sizeof(char));
	
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz=sizeof(clnt_adr);

	for(;;) // multi client
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
		{
			char *str=inet_ntoa(clnt_adr.sin_addr);
			printf("Connected client %d \n", ++i);
			printf("Client IP : %s\n", str);
		}
		while((str_len=read(clnt_sock, message, BUF_SIZE))!=0)
		{
			rev = (char*)realloc(rev, sizeof(char) * str_len);
			reverse(rev, message, str_len);
			write(clnt_sock, rev, str_len);
		}
		
		free(rev);
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void reverse(char *rev, char* message, int mes_len)
{
	int start = 0;

	for (int i = mes_len - 2; i >= 0; i--)
	{
		rev[start] = message[i];
		start++;
	}
	rev[start] = '\n';
}
