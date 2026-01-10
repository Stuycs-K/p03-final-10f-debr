#include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

int server_setup() {
    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;
    
    getaddrinfo(NULL, PORT, hints, &results);
    
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    
    int yes = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("setsockopt error: %s\n", strerror(errno));
        exit(1);
    }
    
    int err = bind(sd, results->ai_addr, results->ai_addrlen);
    if (err == -1) {
        printf("bind error: %s\n", strerror(errno));
        exit(1);
    }
    
    listen(sd, 10);
    
    free(hints);
    freeaddrinfo(results);
    
    return sd;
}

int server_connect(int sd) {
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);
    
    int client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
    return client_socket;
}

int client_connect(char *server_ip) {
    struct addrinfo *hints, *results;
    hints = calloc(1, sizeof(struct addrinfo));
    
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    
    getaddrinfo(server_ip, PORT, hints, &results);
    
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    
    int err = connect(sd, results->ai_addr, results->ai_addrlen);
    if (err == -1) {
        printf("connect error: %s\n", strerror(errno));
        exit(1);
    }
    
    free(hints);
    freeaddrinfo(results);
    
    return sd;
}