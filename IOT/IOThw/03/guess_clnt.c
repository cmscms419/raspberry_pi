#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define WORD_CNT 3
#define WORD_SIZE 7
#define SCORE_MAX 1000

void error_handling(char *message);

void pbm_blank(char pbm[], char blank[]);
void check_pbm(char pbm[], char blank[], char check, int *score);

int main(int argc, char *argv[])
{
    int sock;               // 서버의 소켓
    char message[BUF_SIZE]; // intro를 받는다.
    char pbm[WORD_SIZE];    // 서버로 온 문제를 받는다.
    char blank[WORD_SIZE];  // 문제의 빈 칸이다.
    int str_len;

    int cnt = 0; // 맟춘 문제 수
    char check;

    int score = SCORE_MAX;

    struct sockaddr_in serv_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    // 서버로 부터 게임이 시작됬다는 것을 확인한다.
    str_len = read(sock, message, BUF_SIZE);
    message[str_len] = 0;
    fputs(message, stdout);
    fflush(stdout);

    // 서버로 부터 온 문제를 받는다.
    str_len = read(sock, pbm, WORD_SIZE);

    if (str_len == -1)
        error_handling("read errorr~~~~~~!");

    pbm[str_len] = 0;
    pbm_blank(pbm, blank);

    // 서버로 부터 받아온, 문제를 클라이언트가 푼다.
    while (1)
    {
        printf("%s %d 시도 했습니다,\n", blank, cnt);
        scanf(" %c", &check);

        check_pbm(pbm, blank, check, &score);
        cnt++;

        if (strcmp(pbm, blank) == 0)
        {
            printf("Success!\n");
            printf("스코어 %d\n", score);
            char c[2];
            sprintf(c, "%d", cnt);
            write(sock, c, sizeof(c));
            break;
        }
        else if (score <= 0)
        {
            printf("Fali!\n");
            printf("스코어 %d\n", score);
            write(sock, "-1", 1);
            break;
        }
    }

    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void pbm_blank(char pbm[], char blank[])
{
    for (int i = 0; pbm[i]; i++)
    {
        blank[i] = '_';
    }
}

void check_pbm(char pbm[], char blank[], char check, int *score)
{
    int ck = 0;

    for (int i = 0; pbm[i]; i++)
    {
        if (pbm[i] == check)
        {
            ck++;
            blank[i] = check;

            if (ck <= 1)
            {
                printf("정답 점수 증가 +100: %d\n", (*score) += 100);
            }
        }
    }

    if (ck == 0)
    {
        printf("정답 점수 증가 -250: %d\n", (*score) -= 250);
    }
}