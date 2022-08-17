#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

int main(void)
{
	int fdes1,fdes2,fdes3;
	
	fdes1=socket(PF_INET,SOCK_STREAM,0); //create socket
	fdes2=open("test.dat",O_CREAT);//create file
	fdes3=socket(PF_INET,SOCK_STREAM,0);//create socket
	
	printf("first file discript : %d\n", fdes1);	//3
	printf("second file discipt: %d\n", fdes2);	//4
	printf("third file discipt : %d\n", fdes3);	//5
	
	close(fdes1);
	close(fdes2);
	close(fdes3);

}
