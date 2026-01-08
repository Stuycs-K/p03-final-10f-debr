#ifndef ENTRY_H
#define ENTRY_H

#define MAX_NAME_LEN 64
#define MAX_EMAIL_LEN 128
#define MAX_CLUB_NAME_LEN 64
#define MAX_MEMBERSHIPS 10

struct user_entry {
    int id;
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    int grad_year;
    int memberships[MAX_MEMBERSHIPS];
    int membership_count;
    int active;
};

struct club_entry {
    int id;
    char club_name[MAX_CLUB_NAME_LEN];
    int days[7];
    int active;
};

void user_entry_init(struct user_entry* entry);
void club_entry_init(struct club_entry* entry);
void user_entry_print(const struct user_entry* entry);
void club_entry_print(const struct club_entry* entry);

#endif