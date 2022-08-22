#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 8

char filename[50]="file.txt";
char filesize[20]="0";
void write_file(int sockfd, char recieved_filename[]){
  int cont=0;
  int flag[10];
  int n;
  memset(flag, 0, 10*sizeof(flag[0]));
  int intfilesize=atoi(filesize);
  FILE *fp;
  char *filename = recieved_filename;
  char buffer[SIZE];
  fp = fopen(filename, "w");
  while (1) {
    int progreso=cont*100/intfilesize;
    if(flag[0]==0&&progreso>=10){
        printf("10%c recibido\n",37);
        flag[0]++;
    }
    if(flag[1]==0&&progreso>=20){
        printf("20%c recibido\n",37);
        flag[1]++;
    }
    if(flag[2]==0&&progreso>=30){
        printf("30%c recibido\n",37);
        flag[2]++;
    }
    if(flag[3]==0&&progreso>=40){
        printf("40%c recibido\n",37);
        flag[3]++;
    }
    if(flag[4]==0&&progreso>=50){
        printf("50%c recibido\n",37);
        flag[4]++;
    }
    if(flag[5]==0&&progreso>=60){
        printf("60%c recibido\n",37);
        flag[5]++;
    }
    if(flag[6]==0&&progreso>=70){
        printf("70%c recibido\n",37);
        flag[6]++;
    }
    if(flag[7]==0&&progreso>=80){
        printf("80%c recibido\n",37);
        flag[7]++;
    }
    if(flag[8]==0&&progreso>=90){
        printf("90%c recibido\n",37);
        flag[8]++;
    }
    if(flag[9]==0&&progreso>=100){
        printf("100%c recibido\n",37);
        flag[9]++;
    }
    cont +=8;
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}
void recieve_metadata(int sockfd){
		int n = 0, maxlen = 1024;
		char buffer[maxlen];
		recv(sockfd, buffer, sizeof(char), 0);
		n=atoi(buffer);
        recv(sockfd, buffer,n, 0);
        strcpy(filesize,buffer);
        recv(sockfd, buffer, maxlen, 0);
        strcpy(filename,buffer);
        printf("el archivo sera de tamano %s\n", filesize);
        printf("el archivo se llamara %s\n", filename);
        
}
int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
  int size=0;
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Error en el socket");
    exit(1);
  }
  printf("Socket creado correctamente\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("Error al enlazar");
    exit(1);
  }
  printf("Enlazado exitoso.\n");

  if(listen(sockfd, 10) == 0){
		printf("Escuchando...\n");
	}else{
		perror("Error al escuchar");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  recieve_metadata(new_sock);
  write_file(new_sock, filename);
  printf("Archivo transferido correctamente\n");
  close(new_sock);
  return 0;
}
