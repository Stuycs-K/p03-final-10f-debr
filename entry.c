#include "entry.h"
#include <stdio.h>

#include <string.h>
#include <time.h>

void user_entry_init(struct user_entry* entry) {
    entry->id = -1;
    
    for(int i = 0; i <MAX_NAME_LEN; i++) {
        entry->first_name[i] = 0;
    }
    for(int i = 0; i < MAX_NAME_LEN; i++) {
        entry->last_name[i] = 0;
    }
    for (int i= 0; i < MAX_EMAIL_LEN; i++) {
        entry->email[i] = 0;
    }

    for (int i = 0; i < MAX_PASSWORD_LEN; i++) {
        entry->password[i] = 0;
    }
    
    entry->grad_year = 0;
    entry->is_admin = 0;
    
    for(int i = 0; i < MAX_MEMBERSHIPS; i++) {
        entry->memberships[i] = 0;
    }
    
    entry->membership_count = 0;
    entry->active = 1;
    entry->created_at = time(NULL);
    entry->updated_at = time(NULL);

}

void club_entry_init(struct club_entry* entry) {
    entry->id = -1;
    
    for (int i= 0; i <MAX_CLUB_NAME_LEN; i++) {
        entry->club_name[i] = 0;
    }
    
    for(int i = 0; i < 7; i++) {
        entry->days[i] = 0;
    }

    entry->capacity = 50;
    entry->current_members = 0;
    entry->active = 1;
    entry->created_at = time(NULL);
    entry->updated_at = time(NULL);
}

void user_entry_print(const struct user_entry* entry) {
    printf("User ID: %d\n", entry->id);
    printf("Name: %s %s\n", entry->first_name, entry->last_name);
    printf("Email: %s\n", entry->email);
    printf("Graduation Year: %d\n", entry->grad_year);
    printf("Memberships: ");
    for (int i = 0; i < entry->membership_count; i++) {
        printf("%d ", entry->memberships[i]);
    }
    printf("\n");
}

void club_entry_print(const struct club_entry* entry) {
    printf("Club ID: %d\n", entry->id);
    printf("Name: %s\n", entry->club_name);
    printf("Meeting Days: ");
    const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for(int i = 0; i < 7; i++) {
        if(entry->days[i]) {
            printf("%s ", days[i]);
        }
    }
    printf("\n");
}