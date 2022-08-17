#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>

#define COLOR_S0 0
#define COLOR_S1 1
#define COLOR_S2 2
#define COLOR_S3 3
#define COLOR_OUT 4
#define COLOR_LED 5
char color = 0;
unsigned int countR = 0, countG = 0, countB = 0;
unsigned int counter = 0;

// 인터럽트에 의해 주파수를 측정한다.
void Count(){
	counter ++;
}

void wiringPi_Init(){
	if(wiringPiSetup () == -1)
		exit(1);
	pinMode(COLOR_S0,OUTPUT);
	pinMode(COLOR_S1,OUTPUT);
	pinMode(COLOR_S2,OUTPUT);
	pinMode(COLOR_S3,OUTPUT);
	pinMode(COLOR_OUT,INPUT);
	pinMode(COLOR_LED,OUTPUT);
	
	// S0와 S1의 출력 주파수 비율을 조절한다.
	digitalWrite (COLOR_S0, 0);
	digitalWrite(COLOR_S1, 1);
	
	// 정확한 색상인식을 위해 LED를 켠다.
	digitalWrite(COLOR_LED, 1);
	
	// 인터럽트 선언
	wiringPiISR(COLOR_OUT,INT_EDGE_RISING,&Count);


}

void Detect_Color( char c)
{
	if(c==0) { // RED
	digitalWrite(COLOR_S2, 0);
	digitalWrite(COLOR_S3, 0);
	}
	else if(c==1) { //GREEN
	digitalWrite(COLOR_S2, 1);
	digitalWrite(COLOR_S3, 1);
	}
	else if(c==2) { //BLUE
	digitalWrite(COLOR_S2, 0);
	digitalWrite(COLOR_S3, 1);
	}
}

void *Thread_Func(){
	while(1){
		if(color==0) { //RED
			countR = counter ;
			color = 1; //Request Green
			Detect_Color(color); //S2 =1, S3 = 1
		}
		else if(color==1) { //Green
			countG = counter ;
			color = 2; // Request Blue
			Detect_Color(color); //S2 = 0, S3 =1
		}
		else if(color==2) { //BLEU
			countB = counter ;
			color = 0; //Request Red
			Detect_Color(color); //S2 = 0, S3 = 0
		}
		counter = 0 ;
		delay(1000);
	}
}
	int main(void){
		pthread_t pth_readColor;

		pthread_create(&pth_readColor,0,Thread_Func,0);
		wiringPi_Init();
		delay(1000);
	while(1) {
		printf("Red = %4d ",countR);
		printf("Green = %4d ",countG);
		printf("Blue = %d4 \n",countB);
		delay(1000);
	}
	return 0;
}
