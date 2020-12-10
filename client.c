
/* A simple echo UDP client */

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

    int  sockfd, n, len;
    int sendline[3];
    double recvline;
    struct sockaddr_in servaddr, cliaddr;
    unsigned short port;
    int k, N, split;
    double pi = 0;
    
    printf("Введите k и N:\n");
    scanf("%d", &k);
    scanf("%d", &N);
    
    
    split = (int) N/k;
    
    if(argc < 2 || argc > 3){
       printf("Usage: a.out <IP address> <port - default 51000>\n");
       exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    if(argc == 3){
       port = atoi(argv[2]);
       if(port == 0){
           printf("Invalid port\n");
           exit(-1);
       }
    } else {
       port = 51000;
    }
    servaddr.sin_port   = htons(port);
    
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
       printf("Invalid IP address\n");
       exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
       printf("Can\'t create socket, errno = %d\n", errno);
       exit(1);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family      = AF_INET;
    cliaddr.sin_port        = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
       printf("Can\'t bind socket, errno = %d\n", errno);
       close(sockfd);
       exit(1);
    }

    int ibeg = 0;
    int iend = split - 1;
    printf("---цикл начат---");
    while (ibeg < N) {
        
        sendline[0] = N;
        sendline[1] = ibeg;
        sendline[2] = iend;
        
        printf("N=%d ibeg=%d iend=%d", N, ibeg, iend);
        if(sendto(sockfd, sendline, sizeof(sendline), 0, (struct sockaddr *) &servaddr,
        sizeof(servaddr)) < 0){
           printf("Can\'t send request, errno = %d\n", errno);
           close(sockfd);
           exit(1);
        }
        if((n = recvfrom(sockfd, &recvline, sizeof(double), 0, (struct sockaddr *) NULL, NULL)) < 0){
           printf("Can\'t receive answer, errno = %d\n", errno);
           close(sockfd);
           exit(1);
        }
        ibeg = iend + 1;
        iend = iend + split;
        if (iend > N - 1) {
            iend = N - 1;
        }
        
        printf("вернул: %f\n", recvline);
        
        pi = pi + recvline;
        printf("итерация pi: %f\n", pi);
    }
    printf("---цикл завершен---");
    
    printf("итог pi = %f\n", pi);
    close(sockfd);
    return 0;
}

