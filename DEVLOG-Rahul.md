# Dev Log:

This document must be updated daily every time you finish a work session.

## Rahul Deb

### 2026-01-06 - Finished uploading PROPOSAL and starting makefile + file management
Started writing the overall and technical details of the PROPOSAL and started working on makefile and code for managing the binary files that will be used as tables. Set up initial project structure with entry.h and table.h files.

### 2026-01-07 - Switched to struct-based approach and created entry definitions
Realized that using CSV files would be too complicated. Switched to using binary files with structs for users and clubs. Created the user_entry and club_entry structs with proper fields like ID, names, memberships, etc. Moved struct definitions to header files and wrote initialization functions.

### 2026-01-08 - Implemented entry initialization and print functions
Finished writing the initialization functions for user and club entries. Added print functions to display entry data for debugging. Started planning out what functions I'll need for the table operations.

### 2026-01-09 - Built table operations
Wrote the main table functions: create, insert, read, read_all, update, and delete for both users and clubs. Implemented auto-incrementing ID system by scanning through existing entries. Got the basic file I/O working with binary data.

### 2026-01-10 - Started networking and server setup
Created protocol.h with message types for client-server communication. Wrote networking.c with socket setup functions for both server and client. Started working on server.c and initialized the database with default account and sample clubs. Set up forking to handle multiple clients.

### 2026-01-11 - Completed server handlers and started client interface
Finished writing server-side handlers for register, login, list clubs, join/leave clubs, and view schedule. Started working on client.c with the main menu system. Got basic client-server communication working with the protocol messages.

### 2026-01-12 - Added authentication and admin system
Added password field to user entries and implemented password authentication on login. Created an admin role system with a default admin account (ID 1). Started working on admin-specific features. Fixed some critical bugs with login not making the user session work properly.
