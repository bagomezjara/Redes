#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1"
#define PORT_NO 15050
#define NET_BUF_SIZE 16
#define sendrecvflag 0

void clearBuf(char *b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

int recvFile(char *buf, int s, FILE *fp)
{
    int i;
    char ch;
    for (i = 0; i < s; i++)
    {
        ch = buf[i];
        if (ch == EOF)
            return 1;
        else
            fprintf(fp, "%c", ch);
    }
    return 0;
}
int alt(char *buf, int s, FILE *fp)
{
    char ch;
    fprintf(fp, "%s", buf);
    printf("%s", buf);
    for (int i = 0; i < s; i++)
    {
        ch = buf[i];
        if (ch == EOF)
            return 1;
    }

    return 0;
}

int main()
{
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    char net_buf[NET_BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    if (sockfd < 0)
        printf("\nNo se recibio nombre de archivo\n");
    else
        printf("\nNombre de archivo %d recibido\n", sockfd);

    while (1)
    {
        printf("\nIngresar nombre de archivo:\n");
        scanf("%s", net_buf);
        FILE *fp;
        fp = fopen(net_buf, "w");
        sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr *)&addr_con, addrlen);

        printf("\nDatos recibidos\n");

        while (1)
        {
            clearBuf(net_buf);
            nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr *)&addr_con, &addrlen);

            if (recvFile(net_buf, NET_BUF_SIZE, fp))
            {
                break;
            }
        }
        printf("\n-------------------------------\n");
        fclose(fp);
    }

    return 0;
}
