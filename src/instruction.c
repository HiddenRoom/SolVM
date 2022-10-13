#include "include/instruction.h"

#include <endian.h>
#include <unistd.h>
#include <stdlib.h>

#define INS_VAL be16toh(*(uint16_t *)(&(vm->currentIns[2])))
/* NOTE: input X is NOT the register this is refering
 * to but instead the index of vm->currentIns at which 
 * the number of the desired register will be found */
#define REG(X) vm->regs[vm->currentIns[X]] 

void JMP(vmT *vm) 
{
  vm->execAddr = INS_VAL;
}

void JNE(vmT *vm) 
{
  if(vm->flagReg[0] || vm->flagReg[1])
  {
    vm->execAddr = INS_VAL;
  }
  else
  {
    vm->execAddr += 4;
  }
}

void CMV(vmT *vm) 
{
  /* since 1 1 is never a possibler value for the flag register 
   * conditional jump instructions could just check one byte (excluding
   * JNE) but here both bytes are set if > or < just for clarity */
  if(REG(1) > INS_VAL)
  {
    vm->flagReg[0] = 1;
    vm->flagReg[1] = 0;
  }
  else if(REG(1) < INS_VAL)
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 1;
  }
  else
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 0;
  }

  vm->execAddr += 4;
}

void CMR(vmT *vm) 
{
  if(REG(1) > REG(3))
  {
    vm->flagReg[0] = 1;
    vm->flagReg[1] = 0;
  }
  else if(REG(1) < REG(3))
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 1;
  }
  else
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 0;
  }

  vm->execAddr += 4;
}

void CMM(vmT *vm) 
{
  if(REG(1) > vm->RAM[INS_VAL])
  {
    vm->flagReg[0] = 1;
    vm->flagReg[1] = 0;
  }
  else if(REG(1) < vm->RAM[INS_VAL])
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 1;
  }
  else
  {
    vm->flagReg[0] = 0;
    vm->flagReg[1] = 0;
  }

  vm->execAddr += 4;
}

void LDV(vmT *vm) 
{
  REG(1) = INS_VAL;

  vm->execAddr += 4;
}

void LDR(vmT *vm) 
{
  REG(1) = REG(3);

  vm->execAddr += 4;
}

void LDM(vmT *vm) 
{
  REG(1) = vm->RAM[INS_VAL];

  vm->execAddr += 4;
}

void STR(vmT *vm) 
{
  vm->RAM[INS_VAL] = REG(1);

  vm->execAddr += 4;
}

void ADD(vmT *vm) 
{
  REG(1) += REG(3);

  vm->execAddr += 4;
}

void SUB(vmT *vm) 
{
  REG(1) -= REG(3);

  vm->execAddr += 4;
}

void BXR(vmT *vm) 
{
  REG(1) ^= REG(3);

  vm->execAddr += 4;
}

void BOR(vmT *vm)
{
  REG(1) |= REG(3);

  vm->execAddr += 4;
}

void BND(vmT *vm)
{
  REG(1) &= REG(3);

  vm->execAddr += 4;
}

void BNT(vmT *vm)
{
  REG(1) = ~(REG(3));

  vm->execAddr += 4;
}

void INT(vmT *vm)
{
  if(vm->devNum > INS_VAL)
  (vm->devs[INS_VAL])(vm);

  vm->execAddr += 4;
}

void HLT(vmT *vm)
{
  exit(vm->RAM[65535]);

  vm->execAddr += 4; /* never happens :v */
}

void NOP(vmT *vm)
{
  vm->execAddr += 4;
}
