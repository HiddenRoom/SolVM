#include "include/assembler.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  uint64_t i;
  uint32_t fdIn = open(argv[1], O_RDONLY);
  uint32_t fileLen = lseek(fdIn, 0, SEEK_END);

  char *inputFile;

  assemblerT *assembler;

  if(argc < 2)
  {
    fprintf(stderr, "No input file given\n");
    return 1;
  }

  inputFile = mmap(
      NULL,
      fileLen,
      PROT_READ,
      MAP_PRIVATE,
      fdIn,
      0);

  assembler = assembler_init(inputFile, fileLen);

  printf("Tokens detected:%d\n\n", assembler->lexedInput->tokenNum);

  printf("Tokens:\n");
  for(i = 0; i < assembler->lexedInput->tokenNum; i++)
  {
    printf("%s\n", assembler->lexedInput->tokenVals[i]);
  }

  printf("\n\n");

  printf("Number of instructions detected:%u", assembler->insNum);

  printf("\n\n");

  parse_tokens_to_ins(assembler);

  printf("First above token parsed to bytecode:\n");
  printf("%#04x %#04x %#04x %#04x\n", assembler->instruct[0], assembler->instruct[1], assembler->instruct[2], assembler->instruct[3]);

  return 0;
}
