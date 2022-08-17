/* ------------------------------------------*/
/* blink_client.c                            */
/* $ gcc blink_client.c -o client -lwiringPi */
/* $ sudo ./client  IP_addr  Port            */
/*-------------------------------------------*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#define LED 5  // BCM 25, GND-GND
int main(int argc, char *argv[])
{
   int x;
   struct sockaddr_in p;
   struct hostent *host;   // DNS processing ---
   char message[100];
   char buf[100];        // Dotted decimal
   char buffer[BUFSIZ];

   if ((x = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
        perror("client: socket");
        return 1;
   }
   //--- DNS ---
   host = gethostbyname(argv[1]);
   if( host == NULL)
   {
       printf("Host not found !!: %s\n\r", argv[1]);
       return;
   }

   bzero((char *)&p, sizeof(p));
   p.sin_family = AF_INET;
   memcpy((char *)&p.sin_addr, host->h_addr, host->h_length);
   p.sin_port = htons(atoi(argv[2]));

   inet_ntop(AF_INET, &p.sin_addr, buf, sizeof(buf));
   printf("Trying %s ...\n", buf);
   if (connect(x, (struct sockaddr *)&p, sizeof(p)) == -1)
   {
        perror("client: connect");
        return 1;
   }
   printf("Connected %s...\n", argv[1]);

   if (wiringPiSetup() == -1)
   {
      return 1;
   }

   //-------- GPIO ------------
   pinMode(LED, OUTPUT);
   digitalWrite(LED, 0);

   for( ; ; )
   {

      //---- turn on LED ----
      digitalWrite(LED, 1);
      strcpy(message, "On");
      printf("%s\n", message);
      if (write(x, message, strlen(message) + 1) == -1)
      {
        perror("client: write");
        return 1;
      }
      delay(3000);          // ms

      //---- turn off LED ----
      digitalWrite(LED, 0);
      strcpy(message, "Off");
      printf("%s\n", message);
      if (write(x, message, strlen(message) + 1) == -1)
      {
        perror("client: write");
        return 1;
      }
      delay(3000);
    }

   close(x);
   return 0;
}

