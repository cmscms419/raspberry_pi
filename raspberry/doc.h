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

#define ULTRA_SONIC_ON 202801
#define ULTRA_SONIC_OFF 202800

#define NOBODY 0

//정규 규칙
#define PIR_D 2

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

#define trigPin 28
#define echoPin 29

#define DC_INA 26
#define DC_INB 23
#define DC_ENCA 9
#define DC_ENCB 7

//특수한 상황에서 쓰는 규칙


//EXCHANGE의 규칙
//2개의 RED wiring Pi 번호를 적는다. 뒤에는 신호에 대한 정보를 적는다.
#define GB_EXCHANGE 5611

char pir_flag = 0;

void PIR_interrupt()
{
	pir_flag = 1;
}