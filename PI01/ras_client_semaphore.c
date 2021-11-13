#include "doc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

void error_handling(char* message);
void led_setting();
void pir_setting();

void* thread_snd(void* arg);
void* thread_rcv(void* arg);

char pir_flag = 0;

sem_t bin_sem;
int number = 0;

void PIR_interrupt()
{
	pir_flag = 1;
}

int main(int argc, char* argv[])
{
	int sock;
	int led = 0;
	int str_len;
	int state = 0;

	char message[BUF_SIZE];
	struct sockaddr_in serv_adr;

	void* thread_result;
	pthread_t t1, t2;

	state = sem_init(&bin_sem, 0, 0);
	if (state != 0) 
	{ 
		puts("세마포어 초기화 실패");    
		exit(1);
	}

	if (wiringPiSetup() == -1)
		return 1;

	//IOT setting
	led_setting();
	pir_setting();

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	while (1)
	{
		// 서버에게 PID 값을 전달한다.
		if (pir_flag == 1)
		{
			printf("PIR Detected !! \n");
			pir_flag = 0;

			sprintf(message, "%d", PIR_ON);
			write(sock, message, BUF_SIZE)
		}
		else {
			printf("PIR Not detect !! \n");

			sprintf(message, "%d", PIR_OFF);
			write(sock, message, BUF_SIZE)
		}

		// usleep(50000); // 0.05

		// 서버에게 LED값을 받는다.
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		led = atoi(message);

		switch (led)
		{
		case GB_EXCHANGE:
			digitalWrite(LED_GREEN, 1);
			digitalWrite(LED_BLUE, 0);

			digitalWrite(LED_GREEN, 0);
			digitalWrite(LED_BLUE, 1);

			break;

		case RED_ON:
			digitalWrite(LED_GREEN, 0);
			digitalWrite(LED_BLUE, 0);
			digitalWrite(LED_RED, 1);

			break;

		default:
			close(sock);
			return 0;
		}

	}

	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void led_setting()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);

	digitalWrite(LED_RED, 0);
	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 0);
}

void pir_setting()
{
	pinMode(PIR_D, INPUT);
	wiringPiISR(PIR_D, INT_EDGE_RISING, &PIR_interrupt);
}