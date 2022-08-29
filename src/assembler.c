#include "include/assembler.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <endian.h>
  
__attribute__((always_inline))inline void free_assembler(assemblerT *assembler)
{
  uint32_t i;

  for(i = 0; i < assembler->lexedInput->tokenNum; i++)
    free(assembler->lexedInput->tokenVals[i]);

  free(assembler->lexedInput->tokenVals);
  free(assembler->lexedInput);
  free(assembler);
}

static uint64_t _skip_whitespace(const char *inputFile, uint64_t offset)
{
  while(!isalnum(inputFile[offset]))
    offset++;

  return offset;
}

static int8_t _reg_get(char *regStr) /* this function could be replaced with a hashTable but that would be less portable and only provide a marginal speed increase */
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

  return -1;
}

static bool _is_ins(char *insStr)
{
  if(strcmp(insStr, "JMP") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "JNE") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "CMP") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "MOV") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "STR") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "ADD") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "SUB") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "BXR") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "BOR") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "BND") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "BNT") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "INT") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "HLT") == 0) 
  {
    return true;
  }
  else if(strcmp(insStr, "NOP") == 0) 
  {
    return true;
  }

  return false;
}

static lexerT *_lexer_init(const char *inputFile, uint64_t fileLen)
{
  uint64_t i;
  uint64_t spaceNum = 0; /* spaceNum will determine tokenNum's value since each token in the inputFile will be separated by at least a space */
  uint64_t readOffset = 0;
  uint16_t tokenValLen = 1;

  lexerT *result = calloc(1, sizeof(struct lexerStruct));
  
  for(i = 0; i < fileLen; i++)
  {
    if(!isalnum(inputFile[i]))
    {
      spaceNum++;

      while(!isalnum(inputFile[i])) /* skip ahead until next token */
        i++;
    }
  }

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

static void _copy_val(assemblerT *assembler)
{
  uint16_t tmpInsHold;

  tmpInsHold = (uint16_t)strtol(assembler->lexedInput->tokenVals[assembler->tokenReadOffset], NULL, 10);
  tmpInsHold = htobe16(tmpInsHold);
  memcpy((assembler->instruct) + 2, &tmpInsHold, sizeof(uint16_t));
}

static void _two_reg(assemblerT *assembler)
{
    assembler->instruct[2] = 0x00; /* unused */

    assembler->tokenReadOffset++;

    assembler->instruct[1] = _reg_get(assembler->lexedInput->tokenVals[assembler->tokenReadOffset]);

    assembler->tokenReadOffset++;

    assembler->instruct[3] = _reg_get(assembler->lexedInput->tokenVals[assembler->tokenReadOffset]);
}

static void _op_parse(assemblerT *assembler, uint8_t regOpcode, uint8_t memOpcode, uint8_t litValOpcode)
{
  if(assembler->lexedInput->tokenVals[assembler->tokenReadOffset + 2][0] == 'R') 
  {
    assembler->instruct[0] = regOpcode;

    _two_reg(assembler);
  }
  else if(isdigit(assembler->lexedInput->tokenVals[assembler->tokenReadOffset + 2][0])) 
  {
    assembler->instruct[0] = litValOpcode;

    assembler->tokenReadOffset++;

    assembler->instruct[1] = _reg_get(assembler->lexedInput->tokenVals[assembler->tokenReadOffset]);

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else
  {
    assembler->instruct[0] = memOpcode;

    assembler->tokenReadOffset++;

    assembler->instruct[1] = _reg_get(assembler->lexedInput->tokenVals[assembler->tokenReadOffset]);

    assembler->tokenReadOffset++;

    assembler->lexedInput->tokenVals[assembler->tokenReadOffset] = assembler->lexedInput->tokenVals[assembler->tokenReadOffset] + 1;
    _copy_val(assembler);
    assembler->lexedInput->tokenVals[assembler->tokenReadOffset] = assembler->lexedInput->tokenVals[assembler->tokenReadOffset] - 1;
  }
}

assemblerT *assembler_init(const char *inputFile, uint64_t fileLen) /* only needs parameters to pass them to lexer_init */
{
  uint32_t i;

  assemblerT *result = calloc(1, sizeof(struct assemblerStruct));

  result->tokenReadOffset = 0; /* zero initialization here is redundant due to calloc but it helps readability */
  result->insNum = 0;

  result->lexedInput = _lexer_init(inputFile, fileLen);

  for(i = 0; i < result->lexedInput->tokenNum; i++)
  {
    if(_is_ins(result->lexedInput->tokenVals[i]))
      result->insNum++;
  }

  return result;
}

void parse_tokens_to_ins(assemblerT *assembler) /* will implement logic for parsing each instruction one at a time */
{
  char **tokenStrs = assembler->lexedInput->tokenVals; /* for readability */
  
  /* This section gets opcodes, this process might be able to be optimized by using a hash table
   * although varied instruction string length might make the difficult.*/
  if(strcmp(tokenStrs[assembler->tokenReadOffset], "JMP") == 0) 
  {
    assembler->instruct[0] = 0x00;
    assembler->instruct[1] = 0x00; /* unused */

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "JNE") == 0) 
  {
    assembler->instruct[0] = 0x01;
    assembler->instruct[1] = 0x00; /* unused */

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "CMP") == 0) 
  {
    _op_parse(assembler, 0x03, 0x04, 0x02);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "MOV") == 0) 
  {
    _op_parse(assembler, 0x06, 0x07, 0x05);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "STR") == 0) 
  {
    assembler->instruct[0] = 0x08;

    assembler->tokenReadOffset++;

    assembler->instruct[1] = _reg_get(tokenStrs[assembler->tokenReadOffset]);

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "ADD") == 0) 
  {
    assembler->instruct[0] = 0x09;

    _two_reg(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "SUB") == 0) 
  {
    assembler->instruct[0] = 0x0A;

    _two_reg(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BXR") == 0) 
  {
    assembler->instruct[0] = 0x0B;

    _two_reg(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BOR") == 0) 
  {
    assembler->instruct[0] = 0x0C;

    _two_reg(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BND") == 0) 
  {
    assembler->instruct[0] = 0x0D;

    _two_reg(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "BNT") == 0) 
  {
    assembler->instruct[0] = 0x0E;
    
    assembler->instruct[1] = _reg_get(tokenStrs[assembler->tokenReadOffset]);

    memset((assembler->instruct) + 2, 0, 2 * sizeof(uint8_t));
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "INT") == 0) 
  {
    assembler->instruct[0] = 0x0F;
    assembler->instruct[1] = 0x00;

    assembler->tokenReadOffset++;

    _copy_val(assembler);
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "HLT") == 0) 
  {
    assembler->instruct[0] = 0x10;
    memset((assembler->instruct) + 1, 0, 3 * sizeof(uint8_t));
  }
  else if(strcmp(tokenStrs[assembler->tokenReadOffset], "NOP") == 0) 
  {
    assembler->instruct[0] = 0x11;
    memset((assembler->instruct) + 1, 0, 3 * sizeof(uint8_t));
  }
  else /* turns anything unrecognized into NOP */
  {
    assembler->instruct[0] = 0x11;
    memset((assembler->instruct) + 1, 0, 3 * sizeof(uint8_t));
  }

  assembler->tokenReadOffset++;
}
