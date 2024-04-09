#pragma once
#include "../context.h"

void allocateDeviceMemory(Ruda_Context* context);
void copyDataToDevice(Ruda_Context* context, void* data) ;
void freeDeviceMemory(Ruda_Context* context);