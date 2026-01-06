CC = gcc
CFLAGS = -Wall -Wextra
OBJ = table.o server.o
TARGET = server

.PHONY: compile run clean

compile: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

server.o: server.c table.h
	$(CC) $(CFLAGS) -c server.c

table.o: table.c table.h
	$(CC) $(CFLAGS) -c table.c

run: compile
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) *.csv
