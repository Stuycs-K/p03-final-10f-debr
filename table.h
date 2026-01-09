#ifndef TABLE_H
#define TABLE_H

#include "entry.h"

int user_table_create(const char* filename);
int club_table_create(const char* filename);

int user_table_insert(const char* filename, struct user_entry* entry);
int club_table_insert(const char* filename, struct club_entry* entry);

int user_table_read(const char* filename, int id, struct user_entry* entry);
int club_table_read(const char* filename, int id, struct club_entry* entry);

int user_table_read_all(const char* filename);
int club_table_read_all(const char* filename);

int user_table_update(const char* filename, int index, struct user_entry* entry);
int club_table_update(const char* filename, int index, struct club_entry* entry);

int user_table_delete(const char* filename, int id);
int club_table_delete(const char* filename, int id);

int user_table_get_next_id(const char* filename);
int club_table_get_next_id(const char* filename);

#endif