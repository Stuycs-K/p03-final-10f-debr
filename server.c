#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "networking.h"
#include "protocol.h"
#include "table.h"
#include "entry.h"

void sighandler(int signo);
void subserver(int client_socket);
void handle_register(int client_socket, struct message *msg);
void handle_login(int client_socket, struct message *msg);
void handle_list_clubs(int client_socket);
void handle_join_club(int client_socket, struct message *msg, int user_id);
void handle_leave_club(int client_socket, struct message *msg, int user_id);
void handle_view_schedule(int client_socket, int user_id);
void init_database();
int find_user_index(int user_id);

int main() {
    signal(SIGINT, sighandler);
    
    init_database();
    
    int sd = server_setup();
    printf("Server listening on port %s\n", PORT);
    
    while (1) {
        int client_socket = server_connect(sd);
        printf("Client connected\n");
        
        int f = fork();
        if (f == 0) {
            close(sd);
            subserver(client_socket);
            exit(0);
        } else {
            close(client_socket);
        }
    }
    
    return 0;
}

void init_database() {
    user_table_create("users.dat");
    club_table_create("clubs.dat");
    
    struct club_entry test;
    if(club_table_read("clubs.dat", 1, &test) == -1) {
        struct club_entry club;
        
        club_entry_init(&club);
        strncpy(club.club_name, "Chess Club", MAX_CLUB_NAME_LEN - 1);
        club.days[1] = 1;
        club.days[3] = 1;
        club_table_insert("clubs.dat", &club);
        
        club_entry_init(&club);
        strncpy(club.club_name, "Robotics Club", MAX_CLUB_NAME_LEN - 1);
        club.days[2] = 1;
        club.days[4] = 1;
        club_table_insert("clubs.dat", &club);
        
        club_entry_init(&club);
        strncpy(club.club_name, "Debate Club", MAX_CLUB_NAME_LEN - 1);
        club.days[0] = 1;
        club.days[5] = 1;
        club.days[6] = 1;
        club_table_insert("clubs.dat", &club);
        
        printf("Initialized database with default clubs\n");
    }
}

void subserver(int client_socket) {
    struct message msg;
    struct response resp;
    int user_id = -1;
    int running = 1;
    
    while (running) {
        int bytes = read(client_socket, &msg, sizeof(struct message));
        if (bytes <= 0) {
            printf("Client disconnected\n");
            break;
        }
        
        switch (msg.type) {
            case MSG_REGISTER:
                handle_register(client_socket, &msg);
                break;
            case MSG_LOGIN:
                handle_login(client_socket, &msg);
                read(client_socket, &user_id, sizeof(int));
                break;
            case MSG_LIST_CLUBS:
                handle_list_clubs(client_socket);
                break;
            case MSG_JOIN_CLUB:
                handle_join_club(client_socket, &msg, user_id);
                break;
            case MSG_LEAVE_CLUB:
                handle_leave_club(client_socket, &msg, user_id);
                break;
            case MSG_VIEW_SCHEDULE:
                handle_view_schedule(client_socket, user_id);
                break;
            case MSG_QUIT:
                running = 0;
                break;
            default:
                resp.status = RESP_ERROR;
                strncpy(resp.data, "Unknown command", MSG_SIZE - 1);
                write(client_socket, &resp, sizeof(struct response));
        }
    }
    
    close(client_socket);
}

void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nServer shutting down\n");
        exit(0);
    }
}