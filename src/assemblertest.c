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

  char *inputFile = mmap(
      NULL,
      fileLen,
      PROT_READ,
      MAP_PRIVATE,
      fdIn,
      0);

  assemblerT *assembler = assembler_init(inputFile, fileLen);

  printf("Tokens detected:%d\n", assembler->lexedInput->tokenNum);

  printf("Tokens:\n");
  for(i = 0; i < assembler->lexedInput->tokenNum; i++)
  {
    printf("%s\n", assembler->lexedInput->tokenVals[i]);
  }

  return 0;
}
