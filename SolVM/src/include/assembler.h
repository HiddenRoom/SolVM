#include <stdint.h>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define INS_NUM 4

typedef struct lexerStruct
{
  uint64_t tokenNum;

  char **tokenVals; //will be array of strings of size tokenNum
} lexerT;

typedef struct assemblerStruct
{
  uint64_t tokenReadOffset;
  lexerT *lexedInput;

  uint8_t instruct[INS_NUM];
} assemblerT;

assemblerT *assembler_init(const char *inputFile, uint64_t fileLen); //only needs parameters to pass them to lexer_init

lexerT *lexer_init(const char *inputFile, uint64_t fileLen);

void parse_tokens_to_ins(assemblerT *assembler); //will implement logic for each instruction

#endif
