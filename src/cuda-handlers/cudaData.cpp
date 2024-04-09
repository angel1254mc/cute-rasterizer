//#include <cuda_runtime.h>
#include "cudaData.h"

// Allocates a buffer able to fit the data inside the currently bound buffer, saves the poiter
// to that newly allocated memory to the bound_buffer->buffer_device_pointer var
void allocateDeviceMemory(Ruda_Context* context) {
    //cudaMalloc(context->bound_buffer->buffer_device_pointer, context->bound_buffer->size);
    // Check for errors and handle appropriately
}

// Copies the given data to the bound buffer's container in GPU space
void copyDataToDevice(Ruda_Context* context, void* data) {
    
    //cudaMemcpy(context->bound_buffer->buffer_device_pointer, data, context->bound_buffer->size, cudaMemcpyHostToDevice);
    // Check for errors
}

// Free GPU memory for the currently bound buffer
void freeDeviceMemory(Ruda_Context* context) {
    //cudaFree(context->bound_buffer->buffer_device_pointer);
}
