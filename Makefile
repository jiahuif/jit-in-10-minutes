CC = gcc
RM = rm -f
CFLAGS = -g -Wall -std=gnu99
LDFLAGS = 

all: p1s
p1s: p1s.o code_block.o code_generation.o memory.o compile.o tier0.o tier1.o
	$(CC) $(CFLAGS) $(LDFLAGS) p1s.o code_block.o code_generation.o memory.o compile.o tier0.o tier1.o -o p1s
p1s.o: p1s.c p1s.h tier0.h tier1.h
	$(CC) -c $(CFLAGS) p1s.c -o p1s.o
code_block.o: code_block.c p1s.h
	$(CC) -c $(CFLAGS) code_block.c -o code_block.o
code_generation.o: code_generation.c p1s.h
	$(CC) -c $(CFLAGS) code_generation.c -o code_generation.o
memory.o: memory.c p1s.h
	$(CC) -c $(CFLAGS) memory.c -o memory.o
compile.o: compile.c p1s.h
	$(CC) -c $(CFLAGS) compile.c -o compile.o
tier0.o: tier0.c tier0.h
	$(CC) -c $(CFLAGS) tier0.c -o tier0.o
tier1.o: tier1.c tier1.h
	$(CC) -c $(CFLAGS) tier1.c -o tier1.o

clean:
	$(RM) *.o p1s

