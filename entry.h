#ifndef ENTRY_H
#define ENTRY_H

#include <time.h>

#define MAX_NAME_LEN 64
#define MAX_EMAIL_LEN 128
#define MAX_CLUB_NAME_LEN 64
#define MAX_MEMBERSHIPS 10

#define MAX_PASSWORD_LEN 64


struct user_entry {
    int id;
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char password[MAX_PASSWORD_LEN];
    int grad_year;
    int is_admin;
    int memberships[MAX_MEMBERSHIPS];
    int membership_count;
    int active;
    time_t created_at;
    time_t updated_at;

};

struct club_entry {
    int id;
    char club_name[MAX_CLUB_NAME_LEN];
    int days[7];
    int capacity;
    int current_members;
    int active;
    time_t created_at;
    time_t updated_at;
};

void user_entry_init(struct user_entry* entry);
void club_entry_init(struct club_entry* entry);
void user_entry_print(const struct user_entry* entry);
void club_entry_print(const struct club_entry* entry);

#endif