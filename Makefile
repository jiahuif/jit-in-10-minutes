CC = gcc
RM = rm -f
CFLAGS = -g -Wall
LDFLAGS = 

all: p1s
p1s: p1s.o block.o memory.o
	$(CC) $(CFLAGS) $(LDFLAGS) p1s.o block.o memory.o -o p1s
p1s.o: p1s.c
	$(CC) -c $(CFLAGS) p1s.c -o p1s.o
block.o: block.c
	$(CC) -c $(CFLAGS) block.c -o block.o
memory.o: memory.c
	$(CC) -c $(CFLAGS) memory.c -o memory.o

clean:
	$(RM) *.o p1s

