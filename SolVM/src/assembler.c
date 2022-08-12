#include "include/assembler.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

uint64_t _skip_whitespace(const char *inputFile, uint64_t offset)
{
  while(!isalnum(inputFile[offset]))
    offset++;

  return offset;
}

assemblerT *assembler_init(const char *inputFile, uint64_t fileLen) //only needs parameters to pass them to lexer_init
{
  assemblerT *result = calloc(1, sizeof(struct assemblerStruct));

  result->tokenReadOffset = 0; //zero initialization here is redundant due to calloc but it helps readability
  memset(result->instruct, 0, INS_NUM * sizeof(uint8_t));

  result->lexedInput = lexer_init(inputFile, fileLen);

  return result;
}

lexerT *lexer_init(const char *inputFile, uint64_t fileLen)
{
  uint64_t i;
  uint64_t spaceNum = 0; //spaceNum will determine tokenNum's value since each token in the inputFile will be separated by at least a space
  uint64_t readOffset = 0;
  uint16_t tokenValLen = 1;

  for(i = 0; i < fileLen; i++)
  {
    if(isspace(inputFile[i]))
      spaceNum++;
  }

  lexerT *result = calloc(1, sizeof(struct lexerStruct));
  
  result->tokenNum = spaceNum;

  result->tokenVals = calloc(result->tokenNum, sizeof(char *));
  
  for(i = 0; i < result->tokenNum; i++)
  {
    readOffset = _skip_whitespace(inputFile, readOffset);
      
    result->tokenVals[i] = calloc(1, sizeof(char));
    tokenValLen = 1;

    while(isalnum(inputFile[readOffset]))
    {
      result->tokenVals[i] = realloc(result->tokenVals[i], tokenValLen * sizeof(char));
      result->tokenVals[i][tokenValLen - 1] = inputFile[readOffset];

      tokenValLen++;
      readOffset++;
    }

    result->tokenVals[i] = realloc(result->tokenVals[i], tokenValLen * sizeof(char));
    result->tokenVals[i][tokenValLen - 1] = '\0';
  }

  return result;
}

void parse_tokens(assemblerT *assembler) //will implement logic for parsing each instruction
{
}
