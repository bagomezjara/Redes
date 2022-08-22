#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SIZE 8
void send_file(FILE *fp, int sockfd, int filesize) {
  int cont = 0;
  int flag[10];
  memset(flag, 0, 10 * sizeof(flag[0]));
  char data[SIZE] = {0};
  while (fgets(data, SIZE, fp) != NULL) {
    int progreso = cont * 100 / filesize;
    if (flag[0] == 0 && progreso >= 10) {
      printf("10%c enviado\n", 37);
      flag[0]++;
    }
    if (flag[1] == 0 && progreso >= 20) {
      printf("20%c enviado\n", 37);
      flag[1]++;
    }
    if (flag[2] == 0 && progreso >= 30) {
      printf("30%c enviado\n", 37);
      flag[2]++;
    }
    if (flag[3] == 0 && progreso >= 40) {
      printf("40%c enviado\n", 37);
      flag[3]++;
    }
    if (flag[4] == 0 && progreso >= 50) {
      printf("50%c enviado\n", 37);
      flag[4]++;
    }
    if (flag[5] == 0 && progreso >= 60) {
      printf("60%c enviado\n", 37);
      flag[5]++;
    }
    if (flag[6] == 0 && progreso >= 70) {
      printf("70%c enviado\n", 37);
      flag[6]++;
    }
    if (flag[7] == 0 && progreso >= 80) {
      printf("80%c enviado\n", 37);
      flag[7]++;
    }
    if (flag[8] == 0 && progreso >= 90) {
      printf("90%c enviado\n", 37);
      flag[8]++;
    }
    if (flag[9] == 0 && progreso >= 100) {
      printf("100%c enviado\n", 37);
      flag[9]++;
    }
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("E: Error al enviar el archivo :(\n");
      exit(1);
    }
    cont += 8;
    bzero(data, SIZE);
  }
}
long int findSize(char file_name[]) {
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL) {
    printf("Su archivo no fue encontrado\n");
    return -1;
  }
  fseek(fp, 0L, SEEK_END);
  long int res = ftell(fp);
  fclose(fp);
  return res;
}
void send_metadata(int sockfd, char filename[], char filesize[]) {
  char *data_to_send;
  char aux[20];
  sprintf(aux, "%ld", strlen(filesize));
  data_to_send = aux;
  if (send(sockfd, data_to_send, strlen(data_to_send), 0) == -1) {
    perror("E: Error al enviar el nombre del archivo.\n");
    exit(1);
  }
  data_to_send = filesize;
  if (send(sockfd, data_to_send, strlen(data_to_send), 0) == -1) {
    perror("E: Error al enviar el nombre del archivo.\n");
    exit(1);
  }
  data_to_send = filename;
  if (send(sockfd, data_to_send, strlen(data_to_send), 0) == -1) {
    perror("E: Error al enviar el nombre del archivo.\n");
    exit(1);
  }
}

int main() {
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
  char filename[50];
  char filesize[20];
  memset(filename, '\0', 50); 
  printf("Ingrese el nombre del archivo: \n");
  scanf("%50s", filename);
  int aux = findSize(filename);
  sprintf(filesize, "%d", aux);
  printf("tamano = %s\n", filesize);
  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error en el socket\n");
    exit(1);
  }
  printf("socket de server creado de forma exitosa\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e == -1) {
    perror("Error en el socket\n");
    exit(1);
  }
  printf("Conexion con el servidor establecida\n");

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error al abrir el achivo\n");
    exit(1);
  }
  send_metadata(sockfd, filename, filesize);
  send_file(fp, sockfd, aux);
  printf("Archivo enviado de manera exitosa\n");

  printf("Cerrando conexion\n");
  close(sockfd);

  return 0;
}
