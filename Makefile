CC=gcc
CFLAGS=-std=gnu89 -march=native -g3 -ggdb -O0 -Wall -Wpedantic -Wextra 

asmtest: src/assembler.c src/include/assembler.h src/assemblertest.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblertest.c -o assemblertest.out

asm: src/assembler.c src/include/assembler.h src/assemblerdriver.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblerdriver.c -o assembler.out

emu: src/VM.c src/include/VM.h src/interrupt.c src/include/interrupt.h src/instruction.c src/include/instruction.h src/VMdriver.c
	$(CC) $(CFLAGS) -c -o VM.o src/VM.c -lraylib
	$(CC) $(CFLAGS) -c -o interrupt.o src/interrupt.c
	$(CC) $(CFLAGS) -c -o instruction.o src/instruction.c
	$(CC) $(CFLAGS) -o VM.out VM.o interrupt.o instruction.o src/VMdriver.c -lpthread -lraylib

clean: 
	rm *.o
	rm *.out
