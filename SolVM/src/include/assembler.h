#include <stdint.h>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define INS_NUM 4

typedef struct lexerStruct
{
  uint32_t tokenNum;

  char **tokenVals; /* array of strings of size tokenNum */
} lexerT;

typedef struct assemblerStruct
{
  uint64_t tokenReadOffset;
  uint32_t insNum;

  lexerT *lexedInput;

  uint8_t instruct[INS_NUM];
} assemblerT;

assemblerT *assembler_init(const char *inputFile, uint64_t fileLen); /* only needs parameters to pass them to _lexer_init */

void parse_tokens_to_ins(assemblerT *assembler); /* will implement logic for each instruction */

#endif
