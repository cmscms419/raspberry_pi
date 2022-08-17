/*
작성일자 : 2021/10/4 ~ 2021/10/11
작성자 : 채민석
프로그램명 : smallDNS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stddef.h>

#define BUF 30
#define MAX 1024

typedef struct hosttable
{
	struct hostent table; // 검색한 domain name 이나 IP address를 저장하는 공간이다
	int hit;			  // 얼마나 검색했는지 횟수를 저장하는 공간이다.
} hosttable;

void error_handling(char *message);

int yorn(char c, char str[], hosttable htable[], struct hostent *myhost, int htai);
int Dcheck(hosttable htable[], char str[], struct hostent *myhost, int htai, char c);

void tablePrint_d(int m, hosttable htable[]);
void tablePrint_a(int m, hosttable htable[]);

void insertionSort(hosttable htable[], int htai);

void hostPrint(char str[], struct hostent *myhost, int htai, hosttable htable[], struct in_addr myinaddr);
void addrPrint(char str[], struct hostent *myhost, int htai, hosttable htable[], struct sockaddr_in addr);

int main(int argc, char *argv[])
{
	struct hostent *myhost;	 // domain name 또는 IP address에 대한 정보를 받는 hostent 변수이다.
	struct in_addr myinaddr; //IP 주소를 저장할 구조체
	struct sockaddr_in addr; // IP address를 받아드리는 sockaddr_in 변수이다.

	hosttable htable[MAX]; // domain name과 IP address의 캐쉬 테이블이다.

	int htai = 0;  // htable에 저장된 캐쉬들의 개수이다.
	char c;		   // domain name 혹은 IP address 을 칠것인지, 타입을 정하는 것이다.
	char str[BUF]; // 찾고자 하는 domain name 이나 IP address를 입력 받는 변수이다.

	while (1)
	{
		printf("domain address(d or D) and IP address(a or A) Q or q exit\n");
		scanf(" %c", &c);

		if (c == 'Q' || c == 'q')
			exit(1);
		else if (c == 'd' || c == 'D')
		{
			printf("Domain Address: ");
			scanf("%s", str);

			if (yorn(c, str, htable, myhost, htai) == 1)
			{
				insertionSort(htable, htai);
				continue;
			}
			else
			{
				hostPrint(str, myhost, htai, htable, myinaddr);
				printf("\nNew Update\n");
				htai++;
			}
		}
		else if (c == 'a' || c == 'A')
		{
			printf("IP Address: ");
			scanf("%s", str);

			if (str == NULL)
			{
				printf("Usage : %s \n", str);
				exit(1);
			}

			if (yorn(c, str, htable, myhost, htai) == 1)
			{
				insertionSort(htable, htai);
				continue;
			}
			else
			{
				addrPrint(str, myhost, htai, htable, addr);
				printf("\nNew Update\n");
				htai++;
			}
		}

		insertionSort(htable, htai);

		for (int i = 0; i < BUF; i++)
		{
			str[i] = ' ';
		}
	}

}
// 에러를 검출해서 프로그램을 끝낸다
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// table에 없는 IP address를 검색해서, 출력하고, 검색결과를 저장한다.
void addrPrint(char str[], struct hostent *myhost, int htai, hosttable htable[], struct sockaddr_in addr)
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(str);
	int i = 0;

	myhost = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);

	if (!myhost)
		error_handling("gethost... error");
	printf("Officially name : %s \n\n", myhost->h_name);
	
	puts("Aliases-----------");

	for (; myhost->h_aliases[i]; i++)
	{
		puts(myhost->h_aliases[i]);
	}

	printf("Address Type : %s \n", myhost->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");

	puts("IP Address--------");
	for (i = 0; myhost->h_addr_list[i]; i++)
	{
		puts( inet_ntoa( *(struct in_addr*)myhost->h_addr_list[i] ));
	}

	puts("");

	htable[htai].table = *myhost;
	htable[htai].hit = 1;
}

// table에 없는 domain name를 검색해서, 출력하고, 검색결과를 저장한다.
void hostPrint(char str[], struct hostent *myhost, int htai, hosttable htable[], struct in_addr myinaddr)
{
	int i;

	//host
	myhost = gethostbyname(str);

	if (myhost == 0)
	{
		printf("erro occurs .. at 'get host by name'.\n\n");
		exit(1);
	}

	// 호스트 이름 출력
	printf("official host name : \t\t %s\n\n", myhost->h_name);
	i = 0;

	puts("Aliases-----------");
	//호스트 별명 출력
	while (myhost->h_aliases[i] != NULL)
	{
		printf("aliases name : \t\t%s\n", myhost->h_aliases[i]);
		i++;
	}

	//호스트 주소체계 출력
	printf("host address type : \t\t%d\n", myhost->h_addrtype);
	//호스트 주소 길이 출력
	printf("length of host address : \t%d\n", myhost->h_length);
	
	//gethostbyname 함수로 전달된 도메인 이름에 해당하는 IP 주소 출력
	i = 0;
	while (myhost->h_addr_list[i] != NULL)
	{
		myinaddr.s_addr = *((u_long *)(myhost->h_addr_list[i]));
		printf("address for host:\t\t%s\n", inet_ntoa(myinaddr));
		i++;
	}

	htable[htai].table = *myhost;
	htable[htai].hit = 1;
}

// 검색하려는 내용이 있으면, 서버를 거치지 않고, 가지고 있는 정보를 출력할 지
// 아니면, 서버에 가서 정보를 가져올 지 정하는 함수이다.
int yorn(char c, char str[], hosttable htable[], struct hostent *myhost, int htai)
{
	int i = Dcheck(htable, str, myhost, htai, c);

	if (i < 0)
		return 0;

	if (c == 'd' || c == 'D')
	{
		printf("yorn\n\n");
		tablePrint_d(i, htable);
		return 1;
	}
	else if (c == 'a' || c == 'A')
	{
		printf("yorn\n\n");
		tablePrint_a(i, htable);
		return 1;
	}

	return 0;
}

// 지금 검색한 내용이 있는지 확인한다.
int Dcheck(hosttable htable[], char str[], struct hostent *myhost, int htai, char c)
{

	if (c == 'D' || c == 'd')
	{
		myhost = gethostbyname(str);

		for (int i = 0; i < htai; i++)
		{
			if (strcmp(htable[i].table.h_name, myhost->h_name) == 0)
				return i;
		}
	}
	else if (c == 'a' || c == 'A')
	{
		struct sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = inet_addr(str);

		myhost = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);

		for (int i = 0; i < htai; i++)
		{
			if (strcmp(htable[i].table.h_name, myhost->h_name) == 0)
				return i;
		}
	}
	return -1;
}

// 검색하려는 domain name을 가지고 있어서, 서버를 거치지 않고, 테이블에서 꺼내는 것입니다.
void tablePrint_d(int m, hosttable htable[])
{
	struct in_addr myinaddr;
	int i = 0;

	// 호스트 공식 이름 출력
	printf("official host name : \t\t %s\n\n", htable[m].table.h_name);

	// 호스트의 또다른 이름
	while (htable[m].table.h_aliases[i] != NULL)
	{
		printf("aliases name : \t\t%s\n", htable[m].table.h_aliases[i]);
		i++;
	}

	// 호스트의 IP 타입
	printf("host address type : \t\t%d\n", htable[m].table.h_addrtype);
	// 호스트 IP 주소의 길이
	printf("length of host address : \t%d\n", htable[m].table.h_length);
	i = 0;

	// gethostbyname() 함수로 전달된 도메인 이름에 해당하는 IP 주소
	while (htable[m].table.h_addr_list[i] != NULL)
	{
		myinaddr.s_addr = *((u_long *)(htable[m].table.h_addr_list[i]));
		printf("address for host:\t\t%s\n", inet_ntoa(myinaddr));
		i++;
	}

	htable[m].hit++;
}

// 검색하려는 IP address을 가지고 있어서, 서버를 거치지 않고, 테이블에서 꺼내는 것입니다.
void tablePrint_a(int m, hosttable htable[])
{
	int i = 0;

	// 호스트 이름 출력
	printf("Officially name : %s \n\n", htable[m].table.h_name);
	puts("Aliases-----------");

	//호스트의 공식 도메인 이름 이외의 다른 이름 출력
	for (;htable[m].table.h_aliases[i]; i++)
	{
		puts(htable[m].table.h_aliases[i]);
	}

	// 호스트 주소체계 출력
	printf("Address Type : %s \n", htable[m].table.h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
	puts("IP Address--------");

	//gethostbyname 함수로 전달된 도메인 이름에 해당하는 IP 주소 출력
	for (i = 0; htable[m].table.h_addr_list[i]; i++)
	{
		puts( inet_ntoa(*(struct in_addr *)htable[m].table.h_addr_list[i] ));
	}
	puts("");

	htable[m].hit++;
}

// 삽입 정렬을 이용해서, 테이블을 검색횟수가 가장 높은 순을 [0]에 가깝게 정렬한다.
void insertionSort(hosttable htable[], int htai)
{

	int y = 0;
	hosttable sub;

	for (int i = 1; i < htai; i++)
	{
		sub = htable[i];
		y = i - 1;

		while (y >= 0 && htable[y].hit < sub.hit)
		{
			htable[y + 1] = htable[y];
			y--;
		}
		htable[y + 1] = sub;
	}
}

