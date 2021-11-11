#include <wiringPi.h>

/*
#define 규칙
	 123456789
	 123 : 하드웨어 번호이다.
	 456 : Wiring Pi 핀 번호이다.
	 789 : 신호에 대한 정보이다.
		001 : on
		000 : off
		011 : on_off exchange

*/

// cmscms419가 정한 규칙
#define RED_ON 004007001
#define RED_OFF 004007000
#define GREEN_ON 005021001
#define GREEN_OFF 005021000
#define BLUE_ON 006022001
#defube BLUE_OFF 006022000
#define PIR_ON 027002001
#define PIR_OFF 027002000

//정규 규칙
#define PIR_D 2

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

//특수한 상황에서 쓰는 규칙

//EXCHANGE의 규칙
//2개의 RED wiring Pi 번호를 적는다. 뒤에는 신호에 대한 정보를 적는다.
#define GB_EXCHANGE 005006011