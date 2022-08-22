#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 16
#define sendrecvflag 0
#define nofile "File Not Found!"

void clearBuf(char *b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

int sendFile(FILE *fp, char *buf, int s)
{
    int i, len;
    if (fp == NULL)
    {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        return 1;
    }

    char ch, ch2;
    for (i = 0; i < s; i++)
    {
        ch = fgetc(fp);
        buf[i] = ch;
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
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf[NET_BUF_SIZE];
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

    if (sockfd < 0)
        printf("\nNo se recibio nombre de archivo\n");
    else
        printf("\nNombre de archivo %d recibido\n", sockfd);

    if (bind(sockfd, (struct sockaddr *)&addr_con, sizeof(addr_con)) == 0)
        printf("\nBindeo exitoso\n");
    else
        printf("\nBindeo fallido\n");

    while (1)
    {
        printf("\nESperando por nombre de archivo...\n");

        clearBuf(net_buf);

        nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr *)&addr_con, &addrlen);

        fp = fopen(net_buf, "r");
        printf("\nNombre de archivo recibido: %s\n", net_buf);
        if (fp == NULL)
            printf("\nError al abrir archivo\n");
        else
            printf("\nArchivo abierto correctamente\n");

        while (1)
        {
            if (sendFile(fp, net_buf, NET_BUF_SIZE))
            {
                sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr *)&addr_con, addrlen);
                break;
            }

            sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr *)&addr_con, addrlen);
            clearBuf(net_buf);
        }
        if (fp != NULL)
            fclose(fp);
    }
    return 0;
}

