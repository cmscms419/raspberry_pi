#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	int i = 0;
	struct in_addr myinaddr; //IP 주소를 저장할 구조체
	struct sockaddr_in serv_adr;
	
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	//host
     	myhost=gethostbyname(argv[1]);
	if(myhost==0)
	{
	printf("erro occurs .. at 'get host by name'.\n\n\n");
	exit(1);
	}
	
	// 호스트 이름 출력
	printf("official host name : \t\t %s\n",myhost->h_name);
	
	//호스트 별명 출력
	while(myhost->h_aliases[i]!=NULL)
	{
		printf("aliases name : \t\t%s\n",myhost->h_aliases[i]);
		i++;
	}
	//호스트 주소체계 출력
		printf("host address type : \t\t%d\n",myhost->h_addrtype);
	//호스트 주소 길이 출력
		printf("length of host address : \t%d\n",myhost->h_length);
	//호스트 주소를 dotted decimal 형태로 출력
		i=0;
	while(myhost->h_addr_list[i]!=NULL)
	{
		myinaddr.s_addr=*((u_long *)(myhost->h_addr_list[i]));
		printf("address for host:\t\t%s\n",inet_ntoa(myinaddr));
		i++;
	}
	
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	while(1) 
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;
		
		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s", message);
		if()
	}
	
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
