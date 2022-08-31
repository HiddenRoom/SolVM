#include "include/VM.h"

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>

vmT *vm_init(uint8_t devNum, device *devs)
{
  vmT *result = calloc(1, sizeof(struct VMStruct));

  result->devNum = devNum;
  result->devs = devs;

  return result;
}

void *vidMemDraw(void *vmPtr)
{
  vmT *vm = (vmT *)vmPtr;

  uint16_t i, j;
  uint8_t *vidMemPtr;

  usleep(10000);

  for(i = 0; i < LINE_WIDTH; i++)
  {
    for(j = 0; j < LINE_WIDTH; j++)
    {
      vidMemPtr = (vm->RAM) + j + (i * LINE_WIDTH);

      DrawPixel(j, i, (Color) {vidMemPtr[0], vidMemPtr[1], vidMemPtr[2], ALPHA_VAL});

    }
  }

  return NULL;
}
