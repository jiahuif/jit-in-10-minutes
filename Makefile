CC = gcc
RM = rm -f
CFLAGS = -g -Wall
LDFLAGS = 

all: p1s
p1s: p1s.o
	$(CC) $(CFLAGS) $(LDFLAGS) p1s.o -o p1s
p1s.o: p1s.c
	$(CC) -c $(CFLAGS) p1s.c -o p1s.o

clean:
	$(RM) *.o p1s

