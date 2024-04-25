#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello, world!";
    write(sock, hello, strlen(hello));
    printf("Response sent\n");
    close(sock);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // attach socket to port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // accept connections
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        // create a new thread for each connection
        pthread_t thread;
        new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        
        // create a new thread for each connection
        if (pthread_create(&thread, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        
        // detach the thread so it doesn't need to be joined
        pthread_detach(thread);
    }
    
    // close the server socket
    return 0;
}
