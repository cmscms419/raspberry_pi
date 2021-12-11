#include "doc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <wiringPi.h>
#include <unistd.h>

#define BUFSIZE 100
void error_handling(char *message);

void *control_tower(void *arg);
void detect_module()
//void *PID_module(void *arg);

void UltraSonic();
void Motor_CW();
void Motor_CCW();
void setting();

int ocl_num = 0;
int distance = 0;
//pthread_mutex_t mutex;

unsigned long int EN_count = 0; // 4체배 엔코더 펄스계수 변수
char clockwise = 0;				// 방향 판단 변수 , 1: 시계방향, 0:반시계 방향

int main(int argc, char **argv)
{
	int sock;
	int str_len;

	pthread_t id_t1, id_t2;
	struct sockaddr_in serv_addr;
	void *t_return;
	//int state;
	int signal;

	//state = pthread_mutex_init(&mutex, NULL);

	if (wiringPiSetup() == -1)
		return -1;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	setting();

	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	// thread 와 mutex를 사용해야 한다.
	pthread_create(&id_t1, NULL, control_tower, (void *)&sock);
	//pthread_create(&id_t2, NULL, PID_module, NULL);
	
	pthread_join(id_t1, &t_return);
	//pthread_join(id_t2, &t_return);

	close(sock);
//	pthread_mutex_destroy(&mutex);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// thread function
void *control_tower(void *arg)
{
	int i;
	char message[BUFSIZE];
	int sock = *((int *)arg);
	int signal;

	while (1)
	{
		while (pir_flag == 0);

		sprintf(message, "%d", PIR_ON);
		write(sock, message, BUFSIZE);

		sleep(2);
		read(sock, message, BUFSIZE);
		printf("%s\n", message);

		signal = atoi(message);

		switch (signal)
		{
		case ULTRA_SONIC_ON:
			detect_module();
			break;
		case NOBODY:
			printf("NO Signal\n");
			break;
		}

		pir_flag = 0;
	}
}

//PID_module function
/*
void *PID_module(void *arg)
{
	while (1)
	{
		switch (pir_flag)
		{
		case 1:
			printf("PIR Detected !! \n");
			pir_flag = 0;
			usleep(100000); // 0.1
			break;
		case 0:
			break;
		}
	}
}
*/

void detect_module()
{
	long startTime, travelTime;
	
	while (1)
	{
		/* code */
		digitalWrite(trigPin, LOW);
		usleep(2);

		digitalWrite(trigPin, HIGH);
		usleep(20);

		digitalWrite(trigPin, LOW);

		while (digitalRead(echoPin) == LOW);
		startTime = micros();

		while (digitalRead(echoPin) == HIGH);
		travelTime = micros() - startTime;

		distance = travelTime * 14 / 1000;
		printf("Distance: %d cm\n", distance);

		usleep(100000); // 0.1 second

		if(distance <= 10 && ocl_num == 1)
			continue;
		else if(distance > 10 && ocl_num == 1)
		{
			Motor_CCW();
			break;
		}
		else if(distance <= 10 && ocl_num == 0)
		{
			Motor_CW();
			ocl_num = 1;
		}
	}
}

void ENCB_interrupt(){
	EN_count++;
}

void ENCA_interrupt(){
	char ENCB ;
	
	if(digitalRead(DC_ENCA)==1) { // A상이 high 일 때
		ENCB = digitalRead(DC_ENCB);
	if(ENCB==1) 
		clockwise = 1; // B상이 high = clockwise
	else 
		clockwise = 0; // B상이 low = Counter clockwise
	}
		EN_count++;
}

void Motor_CCW()
{
	int i;
	for (i = 0; i < 10; i++)
	{
		digitalWrite(DC_INB, 0); // CCW운전
		digitalWrite(DC_INA, 1); // CCW 운전
		sleep(1);

		printf("Motor Encode -> %d\n", EN_count);
		if (clockwise == 1)
			printf("Clockwise\n");
		else
			printf("CountClockwise\n");
	}

	digitalWrite(DC_INA, 0); //모터 정지
	digitalWrite(DC_INB, 0); // 모터 정지
	EN_count = 0;
}

void Motor_CW()
{
	int i;
	for (i = 0; i < 10; i++)
	{
		digitalWrite(DC_INA, 0); // CW운전
		digitalWrite(DC_INB, 1); // CW 운전
		sleep(1);
		printf("Motor Encode -> %ld\n", EN_count);

		if (clockwise == 1)
			printf("Clockwise\n");
		else
			printf("CountClockwise\n");
	}
	digitalWrite(DC_INA, 0); // 모터 정지
	digitalWrite(DC_INB, 0); // 모터 정지
	EN_count = 0;
}

void setting()
{
	// PID moduel
	pinMode(PIR_D, INPUT);
	wiringPiISR(PIR_D, INT_EDGE_RISING, &PIR_interrupt);

	// Ultra sonic
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	// DC motor
	pinMode(DC_INA,OUTPUT);
	pinMode(DC_INB,OUTPUT);
	pinMode(DC_ENCA,INPUT);
	pinMode(DC_ENCB,INPUT);
	digitalWrite(DC_INA,0);
	digitalWrite(DC_INB,0);
	pullUpDnControl(DC_ENCA,PUD_UP); //3.3V Pull Up
	pullUpDnControl(DC_ENCB,PUD_UP); // 3.3V Pull Up
	// 인터럽트 설정, 상승 하강 에지 모두에서 인터럽트 발생

	wiringPiISR(DC_ENCA,INT_EDGE_BOTH,&ENCA_interrupt);
	wiringPiISR(DC_ENCB,INT_EDGE_BOTH,&ENCB_interrupt);
}