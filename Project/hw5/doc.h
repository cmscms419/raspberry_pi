#include <wiringPi.h>
#define CRT_SECURE_NO_WARNINGS

/*
#define 규칙
	하드웨어 번호를 부분은 앞에서 부터 번호를 적는다.
	다른 것은 뒤에서 부터 적는다.
	
	 12 34 56
	 12 : 하드웨어 번호이다.
	 34 : Wiring Pi 핀 번호이다.
	 56 : 신호에 대한 정보이다.
		01 : on
		00 : off
		11 : on_off exchange

*/

// cmscms419가 정한 규칙
#define RED_ON 40701
#define RED_OFF 40700
#define GREEN_ON 52101
#define GREEN_OFF 52100
#define BLUE_ON 62201
#define BLUE_OFF 62200

#define PIR_ON 270201
#define PIR_OFF 270200

//정규 규칙
#define PIR_D 2

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

//특수한 상황에서 쓰는 규칙

//EXCHANGE의 규칙
//2개의 RED wiring Pi 번호를 적는다. 뒤에는 신호에 대한 정보를 적는다.
#define GB_EXCHANGE 5611

// 작동하는 IOT 모듈
void led_setting();
void pir_setting();
void pir_OnLED();
void pir_OffLED();


//  LED Setting
void led_setting()
{
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);

	digitalWrite(LED_RED, 0);
	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 0);
}

// PIR Setting
char pir_flag = 0;

void PIR_interrupt()
{
	pir_flag = 1;
}

void pir_setting()
{
	pinMode(PIR_D, INPUT);
	wiringPiISR(PIR_D, INT_EDGE_RISING, &PIR_interrupt);
}

// PIR 작동할 떄 사용하는 모듈
void pir_OnLED()
{
	digitalWrite(LED_RED, 0);
	
	digitalWrite(LED_GREEN, 1);
	digitalWrite(LED_BLUE, 0);
	
	usleep(500000); // 0.5

	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 1);

}

// PIR 작동할 떄 사용하는 모듈
void pir_OffLED()
{
	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 0);
	digitalWrite(LED_RED, 1);
	
	usleep(500000); // 0.5
}
