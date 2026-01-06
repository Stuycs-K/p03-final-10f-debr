# Final Project Proposal

## Group Members:

Rahul Deb      
 
# Intentions:

A club management system that allows students to register, join clubs, and view their weekly schedules. Admins can create, edit, and delete clubs while managing member capacity.
 
# Intended usage:

The system uses a client-server model where multiple clients can connect simultaneously. 

**Non-admin users can:**
- Register for an account with their name, email, graduation year, and password
- Login with their user ID and password
- Browse available clubs with meeting days and capacity info
- Join clubs
- Leave clubs
- View their weekly schedule showing which clubs meet on which days
- Edit their profile information

**Admin users can:**
- Do everything regular users can do, plus:
- Create new clubs with custom names, meeting days, and capacity limits
- Edit existing clubs (change name, days, capacity)
- Delete clubs (automatically removes them from all members' schedules)

The interface is text-based with numbered menus. Users navigate by entering numbers for their choices.
 
# Technical Details:

**File I/O and Data:**
Tables are stored as binary files (`users.dat` and `clubs.dat`) using structs. Each entry has fields like ID, active status, and timestamps. The system reads/writes directly to these files for all operations.

Files used:
- `table.c` / `table.h` - file operations for user and club tables
- `entry.c` / `entry.h` - struct definitions and initialization

**Networking:**
Uses TCP sockets to allow multiple clients to connect to a single server. The server forks child processes to handle each client independently, so multiple people can use the system at once.

Files used:
- `server.c` - handles incoming connections and processes requests
- `client.c` - user interface and sends commands to server
- `networking.c` / `networking.h` - socket setup and connection functions

**Processes:**
The server uses `fork()` to create a subprocess for each connected client. This lets multiple users interact with the database simultaneously without blocking each other.

**Protocol:**
Custom message protocol with defined message types (register, login, join club, etc.) and response codes. Messages are sent as structs over sockets.

Files used:
- `protocol.h` - defines message types and response structures

**Extra Features:**
- Password authentication for login
- Admin role system (admin has ID 1 by default)
- Capacity checking (clubs can't exceed their member limit)
- Automatic cleanup when clubs are deleted (removes from all user schedules)
- Timestamps for when entries are created/updated
- Default clubs created on first server startup
    
# Intended pacing:

- **01/06**: Set up basic file structure, makefile, and networking code
- **01/07**: Implement user registration and login with file I/O
- **01/08**: Add club creation and listing functionality
- **01/09**: Implement join/leave club features
- **01/10**: Add schedule viewing
- **01/13**: Implement admin features (create/edit/delete clubs)
- **01/14**: Add profile editing and capacity limits
- **01/15**: Bug fixes, testing, and documentation