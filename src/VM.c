#include "include/VM.h"

#include <stdint.h>
#include <stdlib.h>
#include <GL/glut.h>

vmT *vm_init(uint8_t devNum, device *devs)
{
  vmT *result = calloc(1, sizeof(struct VMStruct));

  result->devNum = devNum;
  result->devs = devs;

  return result;
}

void vidMemDraw(vmT *vm)
{
  //();
}
