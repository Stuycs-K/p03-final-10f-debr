# Dev Log:

This document must be updated daily every time you finish a work session.

## Rahul Deb

### 2026-01-06 - Finished uploading PROPOSAL and starting makefile + file management
Started writing the overall and technical details of the PROPOSAL and started working on makefile and code for managing the binary files that will be used as tables. Set up initial project structure with entry.h and table.h files.

### 2026-01-07 - Switched to struct-based approach and created entry definitions
Realized that using CSV files would be too complicated. Switched to using binary files with structs for users and clubs. Created the user_entry and club_entry structs with proper fields like ID, names, memberships, etc. Moved struct definitions to header files and wrote initialization functions.

### 2026-01-08 - Implemented entry initialization and print functions
Finished writing the initialization functions for user and club entries. Added print functions to display entry data for debugging. Started planning out what functions I'll need for the table operations.
