#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    int    sockfd, condition;
    char   sendline[1000],getline[1000];
    struct sockaddr_in servaddr;
    unsigned short port;

    if(argc < 2 || argc > 3){
       printf("Usage: a.out <IP address> <port - default 51000>\n");
       exit(1);
    }
    bzero(sendline,1000);
    bzero(getline,1000);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    if(argc == 3){
       port = atoi(argv[2]);
       if(port == 0){
           printf("Invalid port\n");
           exit(-1);
       }
    }
    else{
       port = 51000;
    }
    servaddr.sin_port = htons(port);

    if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
       printf("Invalid IP address\n");
       exit(1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
       printf("Can\'t create socket, errno = %d\n", errno);
       exit(1);
    }
    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
       printf("Can\'t connect, errno = %d\n", errno);
       close(sockfd);
       exit(1);
    }
    while (1){
       printf("String => ");
       scanf("%s", &sendline);

       if (strcmp(sendline, "quit") == 0) {
          printf("Closing connection\n");
          break;
       }
       if ((condition = write(sockfd,sendline, strlen(sendline)+1)) < 0){
          printf("Can\'t write, errno = %d\n", errno);
          close(sockfd);
          exit(1);
       }
       condition = read(sockfd, getline, 1000);
       if (condition < 0){
          printf("Can\'t read, errno = %d\n", errno);
          close(sockfd);
          exit(1);
       }
       if (condition == 0) {
         printf("Connection closed\n");
         break;
       }
       printf("%s\n", getline);
    }
    close(sockfd);
    return 0;
}
