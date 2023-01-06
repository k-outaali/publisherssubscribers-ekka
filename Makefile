
CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -g


all: src/pubsub.c
	$(CC) $(CFLAGS) -c src/pubsub.c -o build/pubsub.o
	$(CC) $(CFLAGS) -c src/linked_list.c -o build/linked_list.o
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o
	$(CC) -o build/main build/main.o build/linked_list.o build/pubsub.o

clean:
	rm build/pubsub.o