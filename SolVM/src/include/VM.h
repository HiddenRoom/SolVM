#include "assembler.h"
#include "output.h"

#include <stdint.h>

#ifndef VM_H
#define VM_H

#define SXTN_BIT_MAX 65536
#define REG_NUM 8

typedef void *(*outputDevice)(vmT *);

typedef struct VMStruct
{
  uint8_t RAM[SXTN_BIT_MAX];
  uint16_t regs[REG_NUM];

  uint8_t currentIns[INS_NUM];
  uint16_t execAddr;

  uint16_t outDevNum;
  outputDevice *outDevs; /* will be called by int instructions and must be defined in src/output.c and src/include/output.h */
} vmT;

vmT *vm_init(uint8_t outDevNum, outputDevice *outDevs);

#endif
