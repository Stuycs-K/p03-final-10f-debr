CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: compile clean server client

compile: server client

server: server.o networking.o table.o entry.o
	$(CC) $(CFLAGS) -o server server.o networking.o table.o entry.o

client: client.o networking.o
	$(CC) $(CFLAGS) -o client client.o networking.o

server.o: server.c networking.h protocol.h table.h entry.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c networking.h protocol.h entry.h
	$(CC) $(CFLAGS) -c client.c

networking.o: networking.c networking.h
	$(CC) $(CFLAGS) -c networking.c

table.o: table.c table.h entry.h
	$(CC) $(CFLAGS) -c table.c

entry.o: entry.c entry.h
	$(CC) $(CFLAGS) -c entry.c

clean:
	rm -f *.o server client users.dat clubs.dat