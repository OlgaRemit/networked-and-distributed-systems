
/* A simple echo UDP server */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

double H(double R, double l) {
    double res = R * R - l * l;
    if (res < 0.0) {
        res = 0.0;
    }
    return sqrt(res);
}


double S(int N, int a, int b) {
    double res = 0;
    double rad = 2.0;
    double atomLen = rad / (double)N;
    int i = a;
    for (; i <= b; ++i) {
        res += H(rad, ((double)i + 0.5) * atomLen) * atomLen;
    }
    return res;
}

int main()
{

    int  sockfd, clilen, n;
    int recvline[3];
    double sendline;
    struct sockaddr_in servaddr, cliaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(51228);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
       printf("Can\'t create socket, errno = %d\n", errno);
       exit(1);
    }

    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
       printf("Can\'t bind socket, errno = %d\n", errno);
       close(sockfd);
       exit(1);
    }

    while(1) {
       
       clilen = sizeof(cliaddr);
       
       if( ( n = recvfrom(sockfd, recvline, sizeof(recvline), 0, (struct sockaddr *) &cliaddr,
       &clilen)) < 0){
          printf("Can\'t receive request, errno = %d\n", errno);
          close(sockfd);
          exit(1);
       }
        
       sendline = S(recvline[0], recvline[1], recvline[2]);
        
       printf("from %d got %d %d %d\n",inet_ntoa(cliaddr.sin_addr), recvline[0], recvline[1], recvline[2]);
       if(sendto(sockfd, &sendline, sizeof(double), 0, (struct sockaddr *) &cliaddr,
       clilen) < 0){
          printf("Can\'t send answer, errno = %d\n", errno);
          close(sockfd);
          exit(1);
       }
    }
    return 0;
}

