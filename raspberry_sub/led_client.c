#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "doc.h"

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int led = 0;
	int str_len;
	struct sockaddr_in serv_adr;
	
	if(wiringPiSetup () == -1)
	return 1;
	pinMode(LED_RED,OUTPUT);
	pinMode(LED_GREEN,OUTPUT);
	pinMode(LED_BLUE,OUTPUT);
	
	digitalWrite(LED_RED,0);
	digitalWrite(LED_GREEN,0);
	digitalWrite(LED_BLUE,0);

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
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	while(1) 
	{
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;
		led = atoi(message);
		
		switch (led)
		{
		case 1 :

			break;

		case 2:

			break;

		case 3:

			break;
		}
		swi (led == 1)
		{
			printf("Red LED On !! \n");
			digitalWrite(LED_GREEN,0);
			digitalWrite(LED_BLUE,0);
			digitalWrite(LED_RED,1);
			usleep(500000); // 0.5
		} 
		else if (led == 2)
		{
			printf("Green LED On !!\n");
			digitalWrite(LED_RED,0);
			digitalWrite(LED_BLUE,0);
			digitalWrite(LED_GREEN,1);
			usleep(500000); // 0.5
		}
		else if	(led == 3)
		{
			printf("Blue LED On !!\n");
			digitalWrite(LED_GREEN,0);
			digitalWrite(LED_RED,0);
			digitalWrite(LED_BLUE,1);
			usleep(500000); // 0.5
		}
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
