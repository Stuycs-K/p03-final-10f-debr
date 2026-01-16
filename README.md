[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# EPSILON 2.0

**Team:** Citrus Circuits  
**Team Member:** Rahul Deb
       
## Project Description:

A client-server application that lets students manage their club memberships. Students can register, browse available clubs, join clubs that fit their schedule, and view their weekly commitments. Admins can create and manage clubs with custom meeting schedules and capacity limits.
  
## Video Demo:

## How to Compile and Run

### Compilation:
```bash
make
```

This creates two executables: `server` and `client`

### Running:

**Start the server first:**
```bash
./server
```

The server will listen on port 4567 and initialize the database with a default admin account and some sample clubs.

**Default admin login:**
- User ID: `1`
- Password: `admin123`

**Connect clients (in separate terminals):**
```bash
./client
```

To connect to a server on a different machine:
```bash
./client 
```

### Cleanup:
```bash
make clean
```

This removes all object files and executables. If you want to reset the database, manually delete `users.dat` and `clubs.dat`.

## Usage Example

1. Start the server
2. Register a new account from the client
3. Note your assigned user ID
4. Login with your ID and password
5. Browse clubs and join ones that interest you
6. Check your weekly schedule to see when clubs meet
7. Admins can login with ID 1 to manage clubs