#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "networking.h"
#include "protocol.h"
#include "entry.h"

int user_id = -1;
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