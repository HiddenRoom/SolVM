#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "VM.h"

void JMP(vmT *vm);

void JNE(vmT *vm);

<<<<<<< HEAD
void CMV(vmT *vm);

void CMR(vmT *vm);

void CMM(vmT *vm);
=======
void CMP(vmT *vm);
>>>>>>> 21ae2f7d7b01c7b52329535c0f260c71c6517183

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
