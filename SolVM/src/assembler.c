#include "include/assembler.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <endian.h>

uint64_t _skip_whitespace(const char *inputFile, uint64_t offset)
{
  while(!isalnum(inputFile[offset]))
    offset++;

  return offset;
}

void _copy_val(assemblerT *assembler)
{
  uint16_t tmpInsHold;

  tmpInsHold = (uint16_t)strtol(assembler->lexedInput->tokenVals[assembler->tokenReadOffset], NULL, 10);
  tmpInsHold = htobe16(tmpInsHold);
  memcpy((assembler->instruct) + 2, &tmpInsHold, sizeof(uint16_t));
}

uint8_t _reg_get(char *regStr) //this function could be replaced with a hashTable but that would be less portable and only provide a marginal speed increase
{
  switch(regStr[1])
  {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
  }
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
    if(!isalnum(inputFile[i]))
    {
      spaceNum++;

      while(!isalnum(inputFile[i]))
        i++;
    }
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

void parse_tokens_to_ins(assemblerT *assembler) //will implement logic for parsing each instruction one at a time
{
  char **tokenStrs = assembler->lexedInput->tokenVals; //for readability
  
  /* This section gets opcodes, this process might be able to be optimized by using a hash table
   * although varied instruction string length might make the difficult.*/
  if(strcmp(tokenStrs[assembler->tokenReadOffset], "JMP") == 0) 
  {
    assembler->instruct[0] = 0x00;
    assembler->instruct[1] = 0x00; //unused

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "JNE") == 0) 
  {
    assembler->instruct[0] = 0x01;
    assembler->instruct[1] = 0x00; //unused

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "CMP") == 0) 
  {
    assembler->instruct[0] = 0x02;
    assembler->instruct[2] = 0x00; //unused

    assembler->tokenReadOffset++;

    assembler->instruct[1] = _reg_get(tokenStrs[assembler->tokenReadOffset]);

    assembler->tokenReadOffset++;

    assembler->instruct[3] = _reg_get(tokenStrs[assembler->tokenReadOffset]);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "LDV") == 0) 
  {
    assembler->instruct[0] = 0x03;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "LDR") == 0) 
  {
    assembler->instruct[0] = 0x04;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "LDM") == 0) 
  {
    assembler->instruct[0] = 0x05;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "STR") == 0) 
  {
    assembler->instruct[0] = 0x06;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "ADD") == 0) 
  {
    assembler->instruct[0] = 0x07;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "SUB") == 0) 
  {
    assembler->instruct[0] = 0x08;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BXR") == 0) 
  {
    assembler->instruct[0] = 0x09;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BOR") == 0) 
  {
    assembler->instruct[0] = 0x0A;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BND") == 0) 
  {
    assembler->instruct[0] = 0x0B;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BNT") == 0) 
  {
    assembler->instruct[0] = 0x0C;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "HLT") == 0) 
  {
    assembler->instruct[0] = 0x0D;
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "NOP") == 0) 
  {
    assembler->instruct[0] = 0x0E;
    assembler->instruct[1] = 0x00;
    assembler->instruct[2] = 0x00;
    assembler->instruct[3] = 0x00;
  }
  else //turns anything unrecognized into NOP
  {
    assembler->instruct[0] = 0x0F;
    assembler->instruct[1] = 0x00;
    assembler->instruct[2] = 0x00;
    assembler->instruct[3] = 0x00;
  }

  assembler->tokenReadOffset++;
}
