#include "include/instruction.h"
#include "include/interrupt.h"
#include "include/VM.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEV_NUM 1

int main(int argc, char **argv)
{
  uint32_t fdIn = open(argv[1], O_RDONLY);
  uint32_t inLen = lseek(fdIn, 0, SEEK_END);
  uint8_t *inputFile;

  vmT *vm;

  device *devs = {ascii_stdout};

  if(argc < 2)
  {
    fprintf(stderr, "USAGE: emulator.out bytecodefile\n");
    return 1;
  }

  inputFile = mmap(
      NULL,
      inLen,
      PROT_READ,
      MAP_PRIVATE,
      fdIn,
      0);

  vm = vm_init(DEV_NUM, devs);

  while(vm->execAddr < inLen) /* TODO: jump instructions may not work with consistent increment */ 
  {
    vm->currentIns = inputFile + vm->execAddr;
    instructions[vm->currentIns](vm);
  }

  return 0;
}
