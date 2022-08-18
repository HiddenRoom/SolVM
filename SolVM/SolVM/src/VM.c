#include "include/VM.h"
#include "include/output.h"

#include <stdint.h>
#include <stdlib.h>

vmT *vm_init(uint8_t outDevNum, outputDevice *outDevs)
{
  vmT *result = calloc(1, sizeof(struct VMStruct));

  result->outDevNum = outDevNum;
  result->outDevs = outDevs;

  return result;
}
