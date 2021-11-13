#include <wiringPi.h>

/*
#define 규칙
	하드웨어 번호를 부분은 앞에서 부터 번호를 적는다.
	다른 것은 뒤에서 부터 적는다.
	
	 123 456 789
	 123 : 하드웨어 번호이다.
	 456 : Wiring Pi 핀 번호이다.
	 789 : 신호에 대한 정보이다.
		001 : on
		000 : off
		011 : on_off exchange

*/

// cmscms419가 정한 규칙
#define RED_ON 400007001
#define RED_OFF 400007000
#define GREEN_ON 500021001
#define GREEN_OFF 500021000
#define BLUE_ON 600022001
#defube BLUE_OFF 600022000

#define PIR_ON 270002001
#define PIR_OFF 270002000

//정규 규칙
#define PIR_D 2

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

//특수한 상황에서 쓰는 규칙

//EXCHANGE의 규칙
//2개의 RED wiring Pi 번호를 적는다. 뒤에는 신호에 대한 정보를 적는다.
#define GB_EXCHANGE 500006011