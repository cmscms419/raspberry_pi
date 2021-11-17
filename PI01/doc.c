#include "doc.h"

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
void pir_setting()
{
	pinMode(PIR_D, INPUT);
	wiringPiISR(PIR_D, INT_EDGE_RISING, &PIR_interrupt);
}

// PIR 작동할 떄 사용하는 모듈
void pir_OnLED()
{
	digitalWrite(LED_GREEN, 1);
	digitalWrite(LED_BLUE, 0);

	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 1);

}

// PIR 작동할 떄 사용하는 모듈
void pir_OffLED()
{
	digitalWrite(LED_GREEN, 0);
	digitalWrite(LED_BLUE, 0);
	digitalWrite(LED_RED, 1);
}