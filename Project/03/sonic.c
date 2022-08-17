#include<stdio.h>
#include<wiringPi.h>

#define trigPin 28
#define echoPin 29

int main(void)
{
	int distance=0,i;
	long startTime, travelTime;
	
	if(wiringPiSetup () == -1)
		return 1;
	
	pinMode (trigPin, OUTPUT);
	pinMode (echoPin, INPUT);
	
	printf("ultra: \n");
	
	while(1) {
		digitalWrite (trigPin, LOW);
		usleep(2);
		
		digitalWrite (trigPin, HIGH);
		usleep(20);
		
		digitalWrite (trigPin, LOW);
		
		while(digitalRead(echoPin) == LOW);
		startTime = micros();
		
		while(digitalRead(echoPin) == HIGH);
		travelTime = micros() - startTime;
		
		distance = travelTime *14 / 1000;
		printf("Distance: %d cm\n", distance);
		
		usleep(100000);
		
	}
}
