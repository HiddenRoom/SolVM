CC=gcc
CFLAGS=-std=gnu89 -g3 -ggdb -O0 -Wall -Wpedantic -Wextra

asmtest: src/assembler.c src/include/assembler.h src/assemblertest.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblertest.c -o assemblertest.out

asm: src/assembler.c src/include/assembler.h src/assemblerdriver.c
	$(CC) $(CFLAGS) -c -o assembler.o src/assembler.c
	$(CC) $(CFLAGS) assembler.o src/assemblerdriver.c -o assembler.out

emutest: src/VM.c src/include/VM.h src/output.c src/include/output.h src/instruction.c src/include/instruction.h src/VMtest.c
	$(CC) $(CFLAGS) -c -o VM.o src/VM.c
	$(CC) $(CFLAGS) -c -o output.o src/output.c
	$(CC) $(CFLAGS) -o VMtest.out VM.o output.o src/VMtest.c

emu: src/VM.c src/include/VM.h src/output.c src/include/output.h src/instruction.c src/include/instruction.h src/VMdriver.c
	$(CC) $(CFLAGS) -c -o VM.o src/VM.c
	$(CC) $(CFLAGS) -c -o output.o src/output.c
	$(CC) $(CFLAGS) -o VM.out VM.o output.o src/VMdriver.c

clean: 
	rm *.o
	rm *.out
