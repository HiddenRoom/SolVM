#ifndef VM_H
#define VM_H

#define SXTN_BIT_MAX 65536
#define REG_NUM 8
#define INS_LEN 4
#define ALPHA_VAL 255
#define LINE_WIDTH 64

#include <stdint.h>
#include <stdbool.h>

typedef void *(*intHandler)(void *);

typedef struct VMStruct
{
  uint8_t RAM[SXTN_BIT_MAX];
  uint16_t regs[REG_NUM];
  uint8_t flagReg[2];

  uint8_t *currentIns;
  uint16_t execAddr;

  uint16_t devNum;
  intHandler *devs; /* will be called by int instructions and must be defined in src/output.c and src/include/output.h */
} vmT;


vmT *vm_init(uint8_t outDevNum, intHandler *devs);

/* void vidMemDraw(vmT *vm); */

#endif
