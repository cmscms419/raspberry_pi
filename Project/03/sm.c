#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define STEP_OUT1A 27
#define STEP_OUT1B 0
#define STEP_OUT2A 1
#define STEP_OUT2B 24

void Moter_CW(int n);
void Moter_CWW(int n);

int main(void)
{
	int i;
	if(wiringPiSetup () == -1)
		return 1;
	pinMode(STEP_OUT1A,OUTPUT);
	pinMode(STEP_OUT1B,OUTPUT);
	pinMode(STEP_OUT2A,OUTPUT);
	pinMode(STEP_OUT2B,OUTPUT);
	
	digitalWrite(STEP_OUT1A,0);
	digitalWrite(STEP_OUT1B,0);
	digitalWrite(STEP_OUT2A,0);
	digitalWrite(STEP_OUT2B,0);
	
	printf("Step Motor Control Start !! \n");
	while(1)
	{
		Moter_CW(100);
		Moter_CWW(100);
	}
	return 0;
}

void Moter_CW(int n)
{
	for(int i = 0; i < n ; i ++) {
			digitalWrite(STEP_OUT2B,1);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,1);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,1);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,1);
			usleep(3000);
		}
		delay(3000);
}


void Moter_CWW(int n)
{
	for(int i = 0; i < n ; i ++) {
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,1);
			usleep(3000);
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,1);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
			digitalWrite(STEP_OUT2B,0);
			digitalWrite(STEP_OUT2A,1);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
			digitalWrite(STEP_OUT2B,1);
			digitalWrite(STEP_OUT2A,0);
			digitalWrite(STEP_OUT1B,0);
			digitalWrite(STEP_OUT1A,0);
			usleep(3000);
		}
		delay(3000);
}
