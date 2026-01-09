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