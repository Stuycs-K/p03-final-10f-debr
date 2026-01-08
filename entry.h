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

#endif