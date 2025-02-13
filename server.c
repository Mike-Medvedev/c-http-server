#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>



#define PORT "8080" 
#define BUFFER_SIZE 100


int main(){
    int status, sockfd, client_connection_sockfd, response_length;
    char buffer[BUFFER_SIZE];
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *result, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo("0.0.0.0", PORT, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    // for(p = result; p != NULL; p = p->ai_next);

    sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(sockfd == -1){
        printf("error making socket");
        exit(1);
    }

    bind(sockfd, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    listen(sockfd, 10);

    printf("Server listening on port %s...\n", "8080");

    while(1){
        addr_size = sizeof their_addr;
        client_connection_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    
    
        char client_ip[INET_ADDRSTRLEN];
        struct sockaddr_in *client_addr = (struct sockaddr_in *)&their_addr;
        inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, INET_ADDRSTRLEN);
    
        printf("Client connected from IP: %s, Port: %d\n", client_ip, ntohs(client_addr->sin_port));
        

        recv(client_connection_sockfd, buffer, BUFFER_SIZE, 0);
        char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";
        send(client_connection_sockfd, response, strlen(response), 0);
    }
   

    close(sockfd);

}