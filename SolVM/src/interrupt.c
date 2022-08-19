#include "include/interrupt.h"
#include "include/VM.h"

#include <stdio.h>

void *ascii_stdout(void *virtMach)
{
  vmT *vm = (vmT *)virtMach;

  printf("%c", vm->regs[0]);
}
