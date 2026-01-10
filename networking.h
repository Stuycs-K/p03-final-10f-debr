#ifndef NETWORKING_H
#define NETWORKING_H

#define PORT "4567"
#define BUFFER_SIZE 1024

int server_setup();
int server_connect(int sd);
int client_connect(char *server_ip);

#endif