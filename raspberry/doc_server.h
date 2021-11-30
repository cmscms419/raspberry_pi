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