#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h> // 좀비 처리
#include <sys/socket.h>

#define BUF_SIZE 100

void error_handling(char *message);
void z_handler(int sig);
int who_win(int a, int b);

int main(int argc, char *argv[])
{
        int fd1[2], fd2[2]; // 두개의 파이프 라인
        char buffer[BUF_SIZE];
        char intro[] = "apple";
        char win[] = "You Win!!\n";
        char lose[] = "You lose!!\n";
        char drawn[] = "You are drawn!!\n";

        int serv_sock, clnt_sock;
        struct sockaddr_in serv_addr, clnt_addr;

        pid_t pid;
        struct sigaction act; // 좀비 처리를 하기위한 act
        int str_len, state, addr_size;

        if (argc != 2)
        {
                printf("Usage : %s <port>\n", argv[0]);
                exit(1);
        }

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

        // 최대 5명까지 기다린다.
        if (listen(serv_sock, 5) == -1)
                error_handling("listen() error");

        while (1)
        {
                addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
                if (clnt_sock == -1)
                        continue;

                pid = fork();
                if (pid == -1)
                {
                        close(clnt_sock);
                        continue;
                }
                else if (pid > 0) /* Parent Process */
                {
                        int result;
                        puts("Connection Created..");
                        close(clnt_sock);

                        write(1, intro, sizeof(intro));
                        sprintf(buffer, "%ld", random() % 3);
                        read(fd1[0], &buffer[1], BUF_SIZE - 1);
                        printf("server :%c client :%c\n", buffer[0], buffer[1]);

                        result = who_win(buffer[0], buffer[1]);
                        if (result == 0)
                        {
                                write(1, drawn, sizeof(drawn));
                                write(fd2[1], drawn, sizeof(drawn));
                        }
                        else if (result == 1)
                        {
                                write(1, win, sizeof(win));
                                write(fd2[1], lose, sizeof(lose));
                        }
                        else
                        {
                                write(1, lose, sizeof(lose));
                                write(fd2[1], win, sizeof(win));
                        }
                }
                else
                { /* Child Process */
                        close(serv_sock);

                        write(clnt_sock, intro, sizeof(intro));
                        read(clnt_sock, buffer, BUF_SIZE);
                        write(fd1[1], buffer, 1);
                        str_len = read(fd2[0], buffer, BUF_SIZE);
                        write(clnt_sock, buffer, str_len);

                        puts("Disconnected..");
                        close(clnt_sock);
                        exit(0);
                }
        }
        return 0;
}

int who_win(int a, int b)
{
        int result;

        if (a == b)
                result = 0;
        else if (a % 3 == (b + 1) % 3)
                result = 1;
        else
                result = -1;

        return result;
}

void z_handler(int sig)
{
        pid_t pid;
        int status;

        pid = waitpid(-1, &status, WNOHANG);
        printf("removed proc id: %d \n", pid);
        printf("Returned data : %d \n\n", WEXITSTATUS(status));
}

void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
