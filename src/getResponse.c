#include "getResponse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>  
#include <netdb.h>

#define PORT 80


int printResponseTest(const char * address, const char * headder);
char* ipToString(struct sockaddr_in *list);

int printResponseTest(const char * address, const char * headder){
    
    int sockfd;
    struct sockaddr_in target;
    struct hostent *hostIp;
    char * p;

    // Get hosts domain name
    p = strchr(address, '/');
    char *dom = malloc(p-address);
    memset(dom, '\0', p-address);
    memcpy(dom, address, p-address);

    // Get server to contact info
    hostIp = gethostbyname(dom);
    if (hostIp == NULL){
        fprintf(stderr,"Failed DNS lookup of %s\n", dom);
        return -1;
    }

    // Create socket 
    sockfd = socket(hostIp->h_addrtype, SOCK_STREAM, 0);
    if (sockfd == -1){
        fprintf(stderr, "socket() failed\n");
        close(sockfd);
        return -1;
    }

    // Clear target & set the address of outgoing message
    memset(&target, 0, sizeof(struct sockaddr_in));
    target.sin_family = hostIp->h_addrtype;
    target.sin_port = htons(PORT);
    memcpy((char *)&target.sin_addr.s_addr,
            hostIp->h_addr_list[0], hostIp->h_length);

    // Connect to host
    if (connect(sockfd, (struct sockaddr *)&target, sizeof(target)) != 0){
        // Failed
        fprintf(stderr, "Failed to connect to: %s\n", ipToString(&target));
        close(sockfd);
        return -1;
    }
    // While connected request a page
    char buff[512];
    //send(sockfd, "POST /t/r1ez2-1565271435/post HTTP/1.1\nContent-Type: text/plain\nAccept: */*\n\nDodgy test",88,0);
    printf("connect to: %s\n", ipToString(&target));
    printf("Header:\n%s<end>\n\n",headder);
    printf("sent:%zd \n",write(sockfd,headder,130));
    recv(sockfd, buff, 511,0);
    printf("recived: \n%s\n",buff);
    close(sockfd);
    return 1;
}

// Print first ip
char * ipToString(struct sockaddr_in *list){
    if (list->sin_family == AF_INET){
        static char IP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &list->sin_addr, IP, INET_ADDRSTRLEN);
        return IP;
    }
    if (list->sin_family == AF_INET){
        static char IP[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &list->sin_addr, IP, INET6_ADDRSTRLEN);
        return IP;
    }
    // ToDo
    return NULL;
}