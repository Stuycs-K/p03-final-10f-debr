#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "networking.h"
#include "protocol.h"
#include "entry.h"

int user_id = -1;
int is_admin = 0;
int server_socket;

void show_main_menu();
void handle_register();
void handle_login();
void handle_list_clubs();
void handle_view_schedule();
void clear_screen();
void pause_screen();

int main(int argc, char *argv[]) {
    char *server_ip = "127.0.0.1";
    
    if (argc == 2) {
        server_ip = argv[1];
    }
    
    server_socket = client_connect(server_ip);
    printf("Connected to server at %s\n", server_ip);
    sleep(1);
    
    show_main_menu();
    
    close(server_socket);
    return 0;
}

void clear_screen() {
    printf("\n\n\n");
}

void pause_screen() {
    printf("\nPress Enter to continue...");
    char c;
    scanf("%c", &c);
}

void show_main_menu() {
    int running = 1;
    int choice;
    
    while (running) {
        clear_screen();
        printf("================================\n");
        printf("   Club Management System\n");
        printf("================================\n\n");
        
        if (user_id == -1) {
            printf("1. Register\n");
            printf("2. Login\n");
            printf("3. Quit\n");
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            char temp[10];
            fgets(temp, 10, stdin);
            
            switch (choice) {
                case 1:
                    handle_register();
                    break;
                case 2:
                    handle_login();
                    break;
                case 3:
                    running = 0;
                    break;
                default:
                    printf("Invalid choice!\n");
                    pause_screen();
            }
        } else {
            printf("1. View Available Clubs\n");
            printf("2. View My Schedule\n");
            printf("3. Logout\n");
            printf("4. Quit\n");
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            char temp[10];
            fgets(temp, 10, stdin);
            
            switch (choice) {
                case 1:
                    handle_list_clubs();
                    break;
                case 2:
                    handle_view_schedule();
                    break;
                case 3:
                    user_id = -1;
                    printf("\nLogged out successfully!\n");
                    pause_screen();
                    break;
                case 4:
                    running = 0;
                    break;
                default:
                    printf("Invalid choice!\n");
                    pause_screen();
            }
        }
    }
    
    struct message msg;
    msg.type = MSG_QUIT;
    write(server_socket, &msg, sizeof(struct message));
}

void handle_register() {
    clear_screen();
    printf("--- Register New Account ---\n\n");
    
    char first[MAX_NAME_LEN], last[MAX_NAME_LEN], email[MAX_EMAIL_LEN], password[MAX_PASSWORD_LEN];
    int grad_year;
    
    printf("First Name: ");
    fgets(first, MAX_NAME_LEN, stdin);
    for (int i = 0; first[i]; i++) {
        if (first[i] == '\n') first[i] = '\0';
    }
    
    printf("Last Name: ");
    fgets(last, MAX_NAME_LEN, stdin);
    for (int i = 0; last[i]; i++) {
        if (last[i] == '\n') last[i] = '\0';
    }
    
    printf("Email: ");
    fgets(email, MAX_EMAIL_LEN, stdin);
    for (int i = 0; email[i]; i++) {
        if (email[i] == '\n') email[i] = '\0';
    }

    printf("Password: ");
    fgets(password, MAX_PASSWORD_LEN, stdin);
    for (int i = 0; password[i]; i++) {
        if (password[i] == '\n') password[i] = '\0';
    }

    printf("Graduation Year: ");
    scanf("%d", &grad_year);
    char temp[10];
    fgets(temp, 10, stdin);
    
    struct message msg;
    msg.type = MSG_REGISTER;
    sprintf(msg.data, "%s %s %s %d", first, last, email, password, grad_year);
    
    write(server_socket, &msg, sizeof(struct message));
    
    struct response resp;
    read(server_socket, &resp, sizeof(struct response));
    
    printf("\n%s\n", resp.data);
    pause_screen();
}

void handle_login() {
    clear_screen();
    printf("--- Login ---\n\n");
    
    int id;
    char password[MAX_PASSWORD_LEN];

    printf("Enter your User ID: ");
    scanf("%d", &id);
    char temp[10];
    fgets(temp, 10, stdin);

    printf("Password: ");
    fgets(password, MAX_PASSWORD_LEN, stdin);
    for (int i = 0; password[i]; i++) {
        if (password[i] == '\n') password[i] = '\0';
    }
    
    struct message msg;
    msg.type = MSG_LOGIN;
    sprintf(msg.data, "%d", id, password);
    
    write(server_socket, &msg, sizeof(struct message));
    
    struct response resp;
    read(server_socket, &resp, sizeof(struct response));
    
    int returned_id;
    read(server_socket, &returned_id, sizeof(int));
    
    if (resp.status == RESP_OK) {
        user_id = returned_id;
        if(strstr(resp.data,"Admin") != NULL) {
            is_admin = 1;
        }
    }
    
    printf("\n%s\n", resp.data);
    pause_screen();
}

void handle_list_clubs() {
    clear_screen();
    
    struct message msg;
    msg.type = MSG_LIST_CLUBS;
    write(server_socket, &msg, sizeof(struct message));
    
    int count;
    read(server_socket, &count, sizeof(int));
    
    if (count == 0) {
        printf("No clubs available.\n");
        pause_screen();
        return;
    }
    
    struct club_entry *clubs = malloc(count * sizeof(struct club_entry));
    
    for (int i = 0; i < count; i++) {
        read(server_socket, &clubs[i], sizeof(struct club_entry));
    }
    
    printf("--- Available Clubs ---\n\n");
    
    const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, clubs[i].club_name);
        printf("   Meets on: ");
        
        int first = 1;
        for (int j = 0; j < 7; j++) {
            if (clubs[i].days[j]) {
                if (!first) printf(", ");
                printf("%s", days[j]);
                first = 0;
            }
        }
        printf("\n\n");
    }
    
    printf("0. Back\n");
    printf("\nSelect a club (or 0 to go back): ");
    
    int choice;
    scanf("%d", &choice);
    char temp[10];
    fgets(temp, 10, stdin);
    
    if (choice > 0 && choice <= count) {
        int club_index = choice - 1;
        
        clear_screen();
        printf("--- %s ---\n\n", clubs[club_index].club_name);
        printf("1. Join Club\n");
        printf("2. Leave Club\n");
        printf("3. Back\n");
        printf("\nEnter choice: ");
        
        int action;
        scanf("%d", &action);
        char temp[10];
        fgets(temp, 10, stdin);
        
        if (action == 1) {
            struct message join_msg;
            join_msg.type = MSG_JOIN_CLUB;
            sprintf(join_msg.data, "%d", clubs[club_index].id);
            write(server_socket, &join_msg, sizeof(struct message));
            
            struct response resp;
            read(server_socket, &resp, sizeof(struct response));
            printf("\n%s\n", resp.data);
            pause_screen();
        } else if (action == 2) {
            struct message leave_msg;
            leave_msg.type = MSG_LEAVE_CLUB;
            sprintf(leave_msg.data, "%d", clubs[club_index].id);
            write(server_socket, &leave_msg, sizeof(struct message));
            
            struct response resp;
            read(server_socket, &resp, sizeof(struct response));
            printf("\n%s\n", resp.data);
            pause_screen();
        }
    }
    
    free(clubs);
}

void handle_view_schedule() {
    clear_screen();
    
    struct message msg;
    msg.type = MSG_VIEW_SCHEDULE;
    write(server_socket, &msg, sizeof(struct message));
    
    int count;
    read(server_socket, &count, sizeof(int));
    
    if (count == 0) {
        printf("You are not in any clubs yet!\n");
        pause_screen();
        return;
    }
    
    printf("--- Your Weekly Schedule ---\n\n");
    
    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    struct club_entry *clubs = malloc(count * sizeof(struct club_entry));
    for (int i = 0; i < count; i++) {
        read(server_socket, &clubs[i], sizeof(struct club_entry));
    }
    
    for (int day = 0; day < 7; day++) {
        int has_clubs = 0;
        
        for (int i = 0; i < count; i++) {
            if (clubs[i].days[day]) {
                if (!has_clubs) {
                    printf("%s:\n", days[day]);
                    has_clubs = 1;
                }
                printf("  - %s\n", clubs[i].club_name);
            }
        }
        
        if (has_clubs) {
            printf("\n");
        }
    }
    
    free(clubs);
    pause_screen();
}