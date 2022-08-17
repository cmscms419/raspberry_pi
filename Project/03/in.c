#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#define KEY_UP 3
#define KEY_DOWN 4
#define KEY_LEFT 5
#define KEY_RIGHT 6
#define SEL 16
#define ADC_CS 8
#define ADC_CH4 4
#define ADC_CH5 5
#define SPI_CH 0
#define SPI_SPEED 500000
char adChannel ;
void Joy_V(){
int adcValue=0;
unsigned char buf[3];
adChannel = ADC_CH4 ;
buf[0] = 0x06 | ((adChannel & 0x07)>>2);
buf[1] = ((adChannel & 0x07)<<6);
buf[2] = 0x00;
digitalWrite(ADC_CS,0);
wiringPiSPIDataRW(SPI_CH,buf,3);
buf[1] = 0x0F & buf[1];
adcValue = (buf[1] << 8) | buf[2];
digitalWrite(ADC_CS,1);
printf("V -> %d ",adcValue);
}
void Joy_H(){
int adcValue=0;
unsigned char buf[3];
adChannel = ADC_CH5 ;
buf[0] = 0x06 | ((adChannel & 0x07)>>2);
buf[1] = ((adChannel & 0x07)<<6);
buf[2] = 0x00;
digitalWrite(ADC_CS,0);
wiringPiSPIDataRW(SPI_CH,buf,3);
buf[1] = 0x0F & buf[1];
adcValue = (buf[1] << 8) | buf[2];
digitalWrite(ADC_CS,1);
printf(" H -> %d\n",adcValue);
}
int main(void)
{
int i,ret=0;
if(wiringPiSetup () == -1)
return 1;
pinMode(KEY_UP,INPUT);
pinMode(KEY_DOWN,INPUT);
pinMode(KEY_LEFT,INPUT);
pinMode(KEY_RIGHT,INPUT);
pinMode(SEL,INPUT);
pinMode(ADC_CS,OUTPUT);
if(wiringPiSPISetup(SPI_CH,SPI_SPEED) == -1) {
printf("wiringPi SPI Setup failed!\n");
exit(0);
}
while(1){
ret = digitalRead(KEY_UP);
if(ret==0)
printf("UP Button push !!\n");
ret = digitalRead(KEY_DOWN);
if(ret==0)
printf("DOWN Button push !!\n");
ret = digitalRead(KEY_LEFT);
if(ret==0)
printf("LEFT Button push !!\n");
ret = digitalRead(KEY_RIGHT);
if(ret==0)
printf("RIGHT Button push !!\n");
ret = digitalRead(SEL);
if(ret==0)
printf("JoyStick Sel Button push !!\n");
Joy_V();
Joy_H();
usleep(500000);
}
return 0;
}
