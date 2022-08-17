#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define WORD_CNT 3
#define WORD_SIZE 7

struct clnt_table
{
    char ip[10]; // IP 저장공간
    int count;   // 성공횟수
    int problem; // 문제 유형
};



void error_handling(char *message);
void z_handler(int sig);

int main(int argc, char *argv[])
{
    // fd2 : 자식 프로세스, fd1 : 부모 프로세스
    int fd1[2], fd2[2]; // 두개의 파이프 라인
    struct clnt_table ct[BUF_SIZE]; // 클라이언트 마다 id와 도전 회수를 저장하는 곳이다.
    
    char Guess[WORD_CNT][WORD_SIZE] = {{"apple"}, {"banana"}, {"orange"}}; // 문제를 낼 단어들이다.
    char clnt_search[WORD_SIZE]; // 자식 클라이어트가 문제를 받는 변수
    char intro[] = "Start Game\n";
    char count; // 정답 시도 횟수
    
    int search; // Guess word 인덱스
    int clnt_part = 0;      // 게임에 참여한 클라이언트

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;

    pid_t pid;
    struct sigaction act; // 좀비 처리를 하기위한 act
    int str_len, state, addr_size;

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        error_handling("Pipe() error!!");

    // 좀비를 처리하기 위한 설정
    act.sa_handler = z_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);
    if (state != 0)
        error_handling("sigaction() error");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // 대결할 2사람을 기다린다.
    if (listen(serv_sock, 2) == -1)
        error_handling("listen() error");

    while (1)
    {
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
        
        if (clnt_sock == -1)
            continue;

        pid = fork();
        
        if(pid==-1)
		{
  		   close(clnt_sock);
		   continue;
		}
        else if(pid > 0)
        {
            // 부모 프로세스
            printf("Parent process\n");
            printf("%s\n", inet_ntoa(clnt_addr.sin_addr));
            strcpy(ct[clnt_part].ip, inet_ntoa(clnt_addr.sin_addr));

            close(clnt_sock);

            // 게임이 시작됬다는 것을 서버에 알려준다.
            write(1, intro, sizeof(intro));

            // 랜덤으로 어떤 한 단어를 선택한다, 어떤 유형인지 저장한다.
            srand(time(NULL));
            search = random() % WORD_CNT;
            ct[clnt_part].problem = search;

            // 자식 프로세스에게 게임의 문제를 알려준다.
            write(fd2[1], Guess[search], WORD_SIZE);

            // 자식 프로세스에게 성공회수를 읽어옵니다.
            read(fd1[0], &count, 1);
            ct[clnt_part].count = atoi(&count);
            printf("IP : %s, 문제: %s ,count : %d \n",ct[clnt_part].ip, Guess[ct[clnt_part].problem], ct[clnt_part].count);

            clnt_part++;
        }
        else
        {
            // 자식 프로세스
            printf("Child Process\n");
            close(serv_sock);

            // 클라이언트에게 게임이 시작됬다고 알려준다.
            write(clnt_sock, intro, sizeof(intro));
            sleep(1); // 부모 프로세스와의 순서를 얽히지 않기위해 1초 정도 기다려 준다.

            // 부모 프로세스로 부터 받은 문제를 클라이언트에게 보낸다.
            int str_len = read(fd2[0], &clnt_search, WORD_SIZE);
            if(write(clnt_sock, &clnt_search, WORD_SIZE) == -1)
                error_handling("write error!!!!");
            
            // 클라이언트로 부터 성공횟수를 얻어낸다.
            str_len = read(clnt_sock, &count, 1);

            // 부모 프로세스에게 성공횟수를 넘겨준다.
            write(fd1[1], &count, str_len);
        }

    }

    close(clnt_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void z_handler(int sig)
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d \n", pid);
    printf("Returned data : %d \n\n", WEXITSTATUS(status));
}