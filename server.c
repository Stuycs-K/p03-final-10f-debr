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

void handle_register(int client_socket, struct message *msg) {
    struct response resp;
    struct user_entry user;
    
    char first[MAX_NAME_LEN], last[MAX_NAME_LEN], email[MAX_EMAIL_LEN];
    int grad_year;
    
    sscanf(msg->data, "%s %s %s %d", first, last, email, &grad_year);
    
    user_entry_init(&user);
    strncpy(user.first_name, first, MAX_NAME_LEN - 1);
    strncpy(user.last_name, last, MAX_NAME_LEN - 1);
    strncpy(user.email, email, MAX_EMAIL_LEN - 1);
    user.grad_year = grad_year;
    
    if (user_table_insert("users.dat", &user) == 0) {
        resp.status = RESP_OK;
        sprintf(resp.data, "Registered! Your ID is %d", user.id);
    } else {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "Registration failed", MSG_SIZE - 1);
    }
    
    write(client_socket, &resp, sizeof(struct response));
}

void handle_login(int client_socket, struct message *msg) {
    struct response resp;
    int user_id;
    struct user_entry user;
    
    sscanf(msg->data, "%d", &user_id);
    
    if (user_table_read("users.dat", user_id, &user) == 0) {
        resp.status = RESP_OK;
        sprintf(resp.data, "Welcome back, %s %s!", user.first_name, user.last_name);
        write(client_socket, &resp, sizeof(struct response));
        write(client_socket, &user_id, sizeof(int));
    } else {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "User not found", MSG_SIZE - 1);
        write(client_socket, &resp, sizeof(struct response));
        user_id = -1;
        write(client_socket, &user_id, sizeof(int));
    }
}

void handle_list_clubs(int client_socket) {
    int fd = open("clubs.dat", O_RDONLY);
    if (fd == -1) {
        int count = 0;
        write(client_socket, &count, sizeof(int));
        return;
    }
    
    struct stat st;
    stat("clubs.dat", &st);
    int total_count = st.st_size / sizeof(struct club_entry);
    
    struct club_entry club;
    int active_count = 0;
    for(int i = 0; i < total_count; i++) {
        read(fd, &club, sizeof(struct club_entry));
        if(club.active) active_count++;
    }
    
    write(client_socket, &active_count, sizeof(int));
    
    lseek(fd, 0, SEEK_SET);
    for(int i = 0; i < total_count; i++) {
        read(fd, &club, sizeof(struct club_entry));
        if(club.active) {
            write(client_socket, &club, sizeof(struct club_entry));
        }
    }
    close(fd);
}

int find_user_index(int user_id) {
    int fd = open("users.dat", O_RDONLY);
    if (fd == -1) return -1;
    
    struct user_entry user;
    int index = 0;
    
    while (read(fd, &user, sizeof(struct user_entry)) > 0) {
        if (user.id == user_id) {
            close(fd);
            return index;
        }
        index++;
    }
    close(fd);
    
    return -1;
}

void handle_join_club(int client_socket, struct message *msg, int user_id) {
    struct response resp;
    
    if (user_id == -1) {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "Please login first", MSG_SIZE - 1);
        write(client_socket, &resp, sizeof(struct response));
        return;
    }
    
    int club_id;
    sscanf(msg->data, "%d", &club_id);
    
    struct user_entry user;
    
    if (user_table_read("users.dat", user_id, &user) == 0) {
        for (int i = 0; i < user.membership_count; i++) {
            if (user.memberships[i] == club_id) {
                resp.status = RESP_ERROR;
                strncpy(resp.data, "Already in this club", MSG_SIZE - 1);
                write(client_socket, &resp, sizeof(struct response));
                return;
            }
        }
        
        if (user.membership_count < MAX_MEMBERSHIPS) {
            user.memberships[user.membership_count++] = club_id;
            
            int index = find_user_index(user_id);
            if (index != -1) {
                user_table_update("users.dat", index, &user);
                resp.status = RESP_OK;
                strncpy(resp.data, "Joined club successfully!", MSG_SIZE - 1);
            } else {
                resp.status = RESP_ERROR;
                strncpy(resp.data, "Update failed", MSG_SIZE - 1);
            }
        } else {
            resp.status = RESP_ERROR;
            strncpy(resp.data, "Maximum clubs reached", MSG_SIZE - 1);
        }
    } else {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "User not found", MSG_SIZE - 1);
    }
    
    write(client_socket, &resp, sizeof(struct response));
}

void handle_leave_club(int client_socket, struct message *msg, int user_id) {
    struct response resp;
    
    if (user_id == -1) {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "Please login first", MSG_SIZE - 1);
        write(client_socket, &resp, sizeof(struct response));
        return;
    }
    
    int club_id;
    sscanf(msg->data, "%d", &club_id);
    
    struct user_entry user;
    
    if (user_table_read("users.dat", user_id, &user) == 0) {
        int found = -1;
        for (int i = 0; i < user.membership_count; i++) {
            if (user.memberships[i] == club_id) {
                found = i;
                break;
            }
        }
        
        if (found != -1) {
            for (int i = found; i < user.membership_count - 1; i++) {
                user.memberships[i] = user.memberships[i + 1];
            }
            user.membership_count--;
            
            int index = find_user_index(user_id);
            if (index != -1) {
                user_table_update("users.dat", index, &user);
                resp.status = RESP_OK;
                strncpy(resp.data, "Left club successfully!", MSG_SIZE - 1);
            } else {
                resp.status = RESP_ERROR;
                strncpy(resp.data, "Update failed", MSG_SIZE - 1);
            }
        } else {
            resp.status = RESP_ERROR;
            strncpy(resp.data, "Not in this club", MSG_SIZE - 1);
        }
    } else {
        resp.status = RESP_ERROR;
        strncpy(resp.data, "User not found", MSG_SIZE - 1);
    }
    
    write(client_socket, &resp, sizeof(struct response));
}

void handle_view_schedule(int client_socket, int user_id) {
    if (user_id == -1) {
        int count = 0;
        write(client_socket, &count, sizeof(int));
        return;
    }
    
    struct user_entry user;
    if (user_table_read("users.dat", user_id, &user) == 0) {
        write(client_socket, &user.membership_count, sizeof(int));
        
        for (int i = 0; i < user.membership_count; i++) {
            struct club_entry club;
            if (club_table_read("clubs.dat", user.memberships[i], &club) == 0) {
                write(client_socket, &club, sizeof(struct club_entry));
            }
        }
    }
}


void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nServer shutting down\n");
        exit(0);
    }
}