#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define SPI_CH 0 // SPI
#define ADC_CH2 2 // AD
#define ADC_CS 8
#define SPI_SPEED 500000 // SPI Speed
#define BUZZER 15

#define LED_RED 7
#define LED_GREEN 21
#define LED_BLUE 22

void buzOn();
void buzOFF();
void ledOff();
void ledOn();
void led_grennOpen(int adcValue);



int main(void){
	
	int adcValue=0, i;
	char adChannel = ADC_CH2;
	unsigned char buf[3];
	
	if(wiringPiSetup () == -1)
		return 1;
	
	pinMode(ADC_CS,OUTPUT);
	pinMode(BUZZER,OUTPUT);
	pinMode(LED_RED,OUTPUT);
	pinMode(LED_GREEN,OUTPUT);

	
	digitalWrite(LED_RED,0);
	digitalWrite(LED_GREEN,0);

	
	printf("Buzzer Control Start !! \n");
	if(wiringPiSPISetup(SPI_CH,SPI_SPEED) == -1){
		printf("wiringPi SPI Setup failed!\n");
		exit(0);
	}
while(1){
	buf[0] = 0x06 | ((adChannel & 0x07)>>2);
	buf[1] = ((adChannel & 0x07)<<6);
	buf[2] = 0x00;
	
	digitalWrite(ADC_CS,0);
	wiringPiSPIDataRW(SPI_CH,buf,3);
	buf[1] = 0x0F & buf[1];
	
	adcValue = (buf[1] << 8) | buf[2];
	digitalWrite(ADC_CS,1);
	led_grennOpen(adcValue);
	usleep(1000000);
	
	}
	return 0;
}


void led_grennOpen(int adcValue)
{
	printf("Sound ADC Value -> %d\n",adcValue);
	
	if (adcValue < 100)
	{
		buzOFF();
		ledOff();
	}
	else
	{
		buzOn();
		ledOn();
	}
}

void buzOn()
{
	printf("BUZZER ON !!!\n");
	digitalWrite(BUZZER,1);

}

void buzOFF()
{
	printf("BUZZER OFF !!!\n");
	digitalWrite(BUZZER,0);

}

void ledOn()
{
	digitalWrite(LED_RED,1);
	digitalWrite(LED_GREEN,0);
	sleep(1);
}


void ledOff()
{
	digitalWrite(LED_RED,0);
	digitalWrite(LED_GREEN,1);
	sleep(1);
}
