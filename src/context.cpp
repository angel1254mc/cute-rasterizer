#include "context.h"
#include "./cuda-handlers/cudaData.h"

Ruda_Context ctx;

Buffer_State::~Buffer_State() {
        if (CUDA_ENABLED) {
            if (buffer_pointer)
                freeDeviceMemory(&ctx);;
        } else {
            if (buffer_pointer)
                delete (float*)buffer_pointer;
        }
        // Everything else is statically allocated,
        // and removal of bound buffers is handled
        // by the overarching rudaDeleteBuffers method
    }