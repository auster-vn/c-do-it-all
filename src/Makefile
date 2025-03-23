CC = gcc
CFLAGS = -Wall -Wextra

all: server

server: server.o hex_to_text.o
	$(CC) $(CFLAGS) -o server server.o hex_to_text.o

server.o: server.c hex_to_text.h
	$(CC) $(CFLAGS) -c server.c

hex_to_text.o: hex_to_text.c hex_to_text.h
	$(CC) $(CFLAGS) -c hex_to_text.c

clean:
	rm -f *.o server

