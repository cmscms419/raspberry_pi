#include <wiringPi.h>

/*
#define ��Ģ
	 123456789
	 123 : �ϵ���� ��ȣ�̴�.
	 456 : Wiring Pi �� ��ȣ�̴�.
	 789 : ��ȣ�� ���� �����̴�.
		001 : on
		000 : off
		011 : on_off exchange

*/

// cmscms419�� ���� ��Ģ
#define RED_ON 004007001
#define RED_OFF 004007000
#define GREEN_ON 005021001
#define GREEN_OFF 005021000
#define BLUE_ON 006022001
#defube BLUE_OFF 006022000
#define PIR_ON 027002001
#define PIR_OFF 027002000

//���� ��Ģ
#define PIR_D 2

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

//Ư���� ��Ȳ���� ���� ��Ģ

//EXCHANGE�� ��Ģ
//2���� RED wiring Pi ��ȣ�� ���´�. �ڿ��� ��ȣ�� ���� ������ ���´�.
#define GB_EXCHANGE 005006011