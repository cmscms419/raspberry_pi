#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define DC_INA 26
#define DC_INB 23
#define DC_ENCA 10
#define DC_ENCB 11

unsigned long int EN_count=0; // 4체배 엔코더 펄스계수 변수
char clockwise = 0; // 방향 판단 변수 , 1: 시계방향, 0:반시계 방향
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

void ENCB_interrupt(){
	EN_count++;
}

void Motor_CW()
{
	int i ;
	for(i=0; i<10; i++){
	digitalWrite(DC_INA,0); // CW운전
	digitalWrite(DC_INB,1); // CW 운전
	sleep(1);
	printf("Motor Encode -> %ld\n",EN_count);
	if(clockwise==1)
		printf("Clockwise\n");
	else 
		printf("CountClockwise\n");
	}
	digitalWrite(DC_INA,0); // 모터 정지
	digitalWrite(DC_INB,0); // 모터 정지
	EN_count = 0;
}
void Motor_CCW()
{
	int i ;
	for(i=0; i<10; i++){
	digitalWrite(DC_INB,0); // CCW운전
	digitalWrite(DC_INA,1); // CCW 운전
	sleep(1);
	
	printf("Motor Encode -> %d\n",EN_count);
	if(clockwise==1)
	 printf("Clockwise\n");
	else
	 printf("CountClockwise\n");
	}
	
	digitalWrite(DC_INA,0); //모터 정지
	digitalWrite(DC_INB,0);// 모터 정지
	EN_count = 0;
	}
	
int main(void){
	char dir = 0;
	if(wiringPiSetup () == -1)
		return 1;
	
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
	
	printf("DC motor Control Start !! \n");
	
	while(1){
		printf("1. CW, 2. CCW 3.exit \n select : ");
		scanf("%d",&dir);
		
		if(dir==1){
			printf("CW Selected !!\n");
			Motor_CW();
		}
		else if(dir==2){
			printf("CCW Selected !!\n");
			Motor_CCW();
		}
		else if(dir==3){
			printf("exit Selected !!\n");
			break;
		}
	}
	return 0;
}

