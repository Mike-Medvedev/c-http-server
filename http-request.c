#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



#define SERVICE "http"
#define BUFFER_SIZE 100


int main(int argc, char *argv[]){
    int status, sockfd, response_length;
    char buffer[BUFFER_SIZE];
    char http_request[BUFFER_SIZE];
    snprintf(http_request, BUFFER_SIZE,
        "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n\r\n",
        argv[1]);

    struct addrinfo hints, *result, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], SERVICE, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    // for(p = result; p != NULL; p = p->ai_next);

    sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(sockfd == -1){
        printf("error making socket");
        exit(1);
    }
    connect(sockfd, result->ai_addr, result->ai_addrlen);

    if((send(sockfd, http_request, strlen(http_request), 0)) == 0){
        printf("Houston we have a problem");
    }
    

    while((response_length = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0){
        buffer[response_length] = '\0';
        printf("%s\n", buffer);
    }


   
    close(sockfd);

    

}