#include "include/assembler.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  uint64_t i;
  uint32_t fdIn = open(argv[1], O_RDONLY);
  uint32_t fdOut = open(argv[2], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  uint32_t inLen = lseek(fdIn, 0, SEEK_END);
  uint32_t outLen;

  char *inputFile;
  uint8_t *outputFile;

  assemblerT *assembler;

  if(argc < 3)
  {
    fprintf(stderr, "USAGE: assembler.out inputfile.asm outputfile.out\n");
    return 1;
  }

  inputFile = mmap(
      NULL,
      inLen,
      PROT_READ,
      MAP_PRIVATE,
      fdIn,
      0);

  assembler = assembler_init(inputFile, inLen);

  outLen = assembler->insNum * (INS_NUM * sizeof(uint8_t));

  outputFile = mmap(
      NULL,
      outLen,
      PROT_WRITE,
      MAP_SHARED,
      fdOut,
      0);

  for(i = 0; i < assembler->insNum; i++)
  {
    parse_tokens_to_ins(assembler);
    memcpy(outputFile + (INS_NUM * (i * sizeof(uint8_t))), assembler->instruct, INS_NUM * sizeof(uint8_t));
  }

  close(fdIn);
  close(fdOut);

  munmap(inputFile, inLen);
  munmap(outputFile, outLen);

  return 0;
}
