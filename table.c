#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

int err() {
    printf("errno %d\n", errno);
    printf("%s\n", strerror(errno));
    return -1;
}

int user_table_create(const char* filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return err();
    return 0;
}

int club_table_create(const char* filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return err();
    return 0;
}

int user_table_insert(const char* filename, struct user_entry* entry) {
    entry->id = user_table_get_next_id(filename);
    entry->active = 1;
    
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(fd == -1) return err();
    
    int bytes = write(fd, entry, sizeof(struct user_entry));    
    if (bytes != sizeof(struct user_entry)) return -1;
    
    printf("Inserted user: ID=%d, Name=%s %s\n",entry->id, entry->first_name, entry->last_name);
    return 0;
}

int club_table_insert(const char* filename, struct club_entry* entry) {
    entry->id = club_table_get_next_id(filename);
    entry->active = 1;
    
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(fd == -1) return err();

    int bytes = write(fd, entry, sizeof(struct club_entry));    
    if(bytes != sizeof(struct club_entry)) return -1;
    
    printf("Inserted club: ID=%d, Name=%s\n", entry->id, entry->club_name);
    return 0;
}


int user_table_read(const char* filename, int id, struct user_entry* entry) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if(stat(filename, &st) == -1) {
        return err();
    }
    
    if (st.st_size == 0) {
        printf("File is empty.\n");
        return -1;
    }
    
    int count = st.st_size / sizeof(struct user_entry);
    struct user_entry *entries = malloc(count * sizeof(struct user_entry));
    if (!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    for(int i = 0; i < count; i++) {
        if(entries[i].id == id && entries[i].active) {
            *entry = entries[i];
            free(entries);
            return 0;
        }
    }
    
    free(entries);
    printf("User with ID %d not found.\n", id);
    return -1;
}

int club_table_read(const char* filename, int id, struct club_entry* entry) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if(stat(filename, &st) == -1) {
        return err();
    }
    
    if(st.st_size == 0) {
        printf("File is empty.\n");
        return -1;
    }
    
    int count = st.st_size / sizeof(struct club_entry);
    struct club_entry *entries = malloc(count * sizeof(struct club_entry));
    if (!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    for(int i = 0; i < count; i++) {
        if(entries[i].id == id && entries[i].active) {
            *entry = entries[i];
            free(entries);
            return 0;
        }
    }
    
    free(entries);
    printf("Club with ID %d not found.\n", id);
    return -1;
}

int user_table_read_all(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if (stat(filename, &st) == -1) {
        return err();
    }
    
    if (st.st_size == 0) {
        printf("No users in database.\n");
        return 0;
    }
    
    int count = st.st_size / sizeof(struct user_entry);
    struct user_entry *entries = malloc(count * sizeof(struct user_entry));
    if(!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    printf("\nAll Users\n");
    for(int i = 0; i< count; i++) {
        if(entries[i].active) {
            printf("%d: ID=%d\tName=%s %s\tEmail=%s\tGrad=%d\n", i, entries[i].id, entries[i].first_name, entries[i].last_name, entries[i].email, entries[i].grad_year);
        }

    }
    
    free(entries);
    return 0;
}

int club_table_read_all(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if (stat(filename, &st) == -1) {
        return err();
    }
    
    if (st.st_size == 0) {
        printf("No clubs in database.\n");
        return 0;
    }
    
    int count = st.st_size / sizeof(struct club_entry);
    struct club_entry *entries = malloc(count * sizeof(struct club_entry));
    if(!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    printf("\nAll Clubs\n");
    const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for(int i = 0; i < count; i++) {
        if(entries[i].active) {
            printf("%d: ID=%d\tName=%s\tDays=", i, entries[i].id, entries[i].club_name);
            for(int j = 0; j < 7; j++) {
                if (entries[i].days[j]) printf("%s ", days[j]);
            }
            printf("\n");
        }

    }
    
    free(entries);
    return 0;
}

int user_table_update(const char* filename, int index, struct user_entry* entry) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if (stat(filename, &st) == -1) {
        return err();
    }
    
    int count = st.st_size / sizeof(struct user_entry);
    struct user_entry *entries = malloc(count * sizeof(struct user_entry));
    if (!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    if(index < 0 || index >= count) {
        printf("Invalid index: %d\n", index);
        free(entries);
        return -1;
    }
    
    entries[index] = *entry;
    
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1) {
        free(entries);
        return err();
    }
    
    write(fd, entries, count * sizeof(struct user_entry));
    free(entries);
    
    printf("Updated user at index %d\n", index);
    return 0;
}

int club_table_update(const char* filename, int index, struct club_entry* entry) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if (stat(filename, &st) == -1) {
        return err();
    }
    
    int count = st.st_size / sizeof(struct club_entry);
    struct club_entry *entries = malloc(count * sizeof(struct club_entry));
    if(!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    if (index < 0 || index >= count) {
        printf("Invalid index: %d\n", index);
        free(entries);
        return -1;
    }
    
    entries[index] = *entry;
    
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1) {
        free(entries);
        return err();
    }
    
    write(fd, entries, count * sizeof(struct club_entry));
    free(entries);
    
    printf("Updated club at index %d\n", index);
    return 0;
}

int user_table_delete(const char* filename, int id) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if(stat(filename, &st) == -1) {
        return err();
    }
    
    int count = st.st_size / sizeof(struct user_entry);
    struct user_entry *entries = malloc(count * sizeof(struct user_entry));
    if(!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(entries[i].id == id && entries[i].active) {
            entries[i].active = 0;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("User with ID %d not found.\n", id);
        free(entries);
        return -1;
    }
    
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd == -1) {
        free(entries);
        return err();
    }
    
    write(fd, entries, count * sizeof(struct user_entry));
    free(entries);
    
    printf("Deleted user with ID %d\n", id);
    return 0;
}

int club_table_delete(const char* filename, int id) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return err();
    
    struct stat st;
    if (stat(filename, &st) == -1) {
        return err();
    }
    
    int count = st.st_size / sizeof(struct club_entry);
    struct club_entry *entries = malloc(count * sizeof(struct club_entry));
    if(!entries) {
        return err();
    }
    
    read(fd, entries, st.st_size);
    
    int found = 0;
    for(int i = 0; i < count; i++) {
        if(entries[i].id == id && entries[i].active) {
            entries[i].active = 0;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Club with ID %d not found.\n", id);
        free(entries);
        return -1;
    }
    
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd == -1) {
        free(entries);
        return err();
    }
    
    write(fd, entries, count * sizeof(struct club_entry));
    free(entries);
    
    printf("Deleted club with ID %d\n", id);
    return 0;
}



int user_table_get_next_id(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if(fd == -1) return 1;
    
    struct stat st;
    if(stat(filename, &st) == -1 || st.st_size == 0) {
        return 1;
    }
    
    int count = st.st_size / sizeof(struct user_entry);
    struct user_entry *entries = malloc(count * sizeof(struct user_entry));
    if (!entries) {
        return 1;
    }
    
    read(fd, entries, st.st_size);
    
    int max_id = 0;
    for(int i = 0; i < count; i++) {
        if (entries[i].id > max_id) {
            max_id = entries[i].id;
        }
    }
    
    free(entries);
    return max_id + 1;
}

int club_table_get_next_id(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return 1;

    struct stat st;
    if (stat(filename, &st) == -1 || st.st_size == 0) {
        return 1;
    }

    
    int count = st.st_size / sizeof(struct club_entry);
    struct club_entry *entries = malloc(count * sizeof(struct club_entry));
    if (!entries) {
        return 1;
    }
    
    read(fd, entries, st.st_size);
    
    int max_id = 0;
    for(int i = 0; i < count; i++) {
        if (entries[i].id > max_id) {
            max_id = entries[i].id;
        }
    }
    
    free(entries);
    return max_id + 1;
}