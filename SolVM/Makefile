CC=gcc
CFLAGS=-std=gnu89 -g3 -ggdb -O0 -Wall -Wpedantic -Wextra

asmtest: src/assembler.c src/include/assembler.h src/assemblertest.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblertest.c -o assemblertest.out

asm: src/assembler.c src/include/assembler.h src/assemblerdriver.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblerdriver.c -o assembler.out

clean: 
	rm *.o
	rm *.out
