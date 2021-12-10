#include "doc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFSIZE 100
void error_handling(char* message);

void* control_tower(void* arg);
void* PID_module(void* arg);
void* UltraSonic_module(void* arg);

int control_num = 0;
pthread_mutex_t mutex;

int main(int argc, char** argv)
{
	int sock;
	int str_len;

	pthread_t id_t1, id_t2, id_t3;
	struct sockaddr_in serv_addr;
	void* t_return;
	int state;
	int signal;

	state = pthread_mutex_init(&mutex, NULL);


	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	// thread 와 mutex를 사용해야 한다.
	pthread_create(&id_t1, NULL, control_tower, (void*)&sock);
	pthread_create(&id_t2, NULL, PID_module, NULL);

	pthread_join(id_t1, &t_return);
	pthread_join(id_t2, &t_return);


	close(sock);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// thread function
void* control_tower(void* arg)
{
	int i;
	char message[BUFSIZE];
	int sock = *((int*)arg);
	int signal;

	while (1)
	{
		sprintf(message, "%d", PIR_ON);
		write(sock, message, BUFSIZE);

		sleep(2);

		read(sock, message, BUFSIZE);
		printf("%s\n", message);
		signal = atoi(message);

		switch (signal)
		{
		case ULTRA_SONIC_ON:
			control_num = 1;
			printf("control_tower :%d\n", control_num);
			break;
		}

		if (control_num != 0);
	}

}

void* PID_module(void* arg)
{
	while (1)
	{
		while (control_num == 1)
		{
			pthread_mutex_lock(&mutex);
			printf("PID_module\n");
			sleep(1);
			control_num = 0;
			pthread_mutex_unlock(&mutex);
		}
	}

}

void* UltraSonic_module(void* arg)
{
	while (control_num == 2)
	{
		/* code */
		sleep(2);
		printf("UltraSonic_module\n");
	}

}