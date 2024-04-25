// including necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

// define the necessary variables
int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "GET / HTTP/1.1\nHost: localhost\n\n";
    char buffer[1024] = {0};
    char method[16], protocol[16], url[1024];
    
    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    // set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // error handling for invalid address
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // send GET message
    send(sock , hello , strlen(hello) , 0 );
    printf("GET message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    
    // parse the response
    sscanf(buffer, "%s %s %s", method, url, protocol);
    printf("Method: %s\n", method);
    printf("Url: %s\n", url);
    pringf("Protocol: %s\n", protocol);

    return 0;
}
