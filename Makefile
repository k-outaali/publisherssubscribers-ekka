
CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -g


all: src/pubsub.c
	$(CC) $(CFLAGS) -c src/pubsub.c -o build/pubsub.o

clean:
	rm build/pubsub.o