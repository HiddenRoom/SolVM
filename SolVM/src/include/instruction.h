#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "VM.h"

void JMP(vmT *vm);

void JNE(vmT *vm);

void CMP(vmT *vm);

void LDV(vmT *vm);

void LDR(vmT *vm);

void LDM(vmT *vm);

void STR(vmT *vm);

void ADD(vmT *vm);

void SUB(vmT *vm);

void BXR(vmT *vm);

void BOR(vmT *vm);

void BND(vmT *vm);

void BNT(vmT *vm);

void INT(vmT *vm);

void HLT(vmT *vm);

void NOP(vmT *vm);

#endif
