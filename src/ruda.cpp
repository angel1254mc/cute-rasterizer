
#include "context.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include "ruda.h"
#include "cuda-handlers/cuda_handlers.h"
#include "software-handlers/software_handlers.h"
void rudaGenBuffers(uint n, uint *buffers) {
    
    // Push back empty state for each new buffer generated
    for (int i = 0; i < n; i++) {
        ctx.buffers.push_back(new Buffer_State());
        buffers[i] = ctx.buffers.size() - 1;
    }

    return;

}

void rudaDeleteBuffers(uint n, uint *buffers) {
    for (int i = 0; i < n; i++) {
        // For each buffer in the buffers array
        Buffer_State* buffer = ctx.buffers[buffers[i]];
        // Check if that buffer is currently bound
        // if so, unbind
        if (buffer->is_bound) {
            ctx.bound_buffer = nullptr;
        }
        delete buffer;
        // NOTE
        // I don't actually delete the index of the array
        // For now, just to make implementation simple
        // However- for large amounts of VBOs being created on each render
        // loop, this is NOT a good strategy at all lol
    }
}

void rudaBindBuffer(Buffer_Target target, uint buffer) {
    if (ctx.buffers.size() <= buffer) {
        throw std::invalid_argument("rudaBindBuffer 'buffer' argument must match a created buffer!");
    }

    // Update bound state on previously bound buffer
    if (ctx.bound_buffer)
        ctx.bound_buffer->is_bound = false;

    ctx.bound_buffer = ctx.buffers.at(buffer);
    // Update the bound buffer's purpose/target
    ctx.bound_buffer_target = target;
    ctx.bound_buffer->is_bound = true;

    if (target == RUDA_VERTEX_BUFFER) {
        ctx.state.vertexBuffer = ctx.bound_buffer;
    } else if (target == RUDA_COLOR_BUFFER) {
        ctx.state.colorBuffer = ctx.bound_buffer;
    } else if (target == RUDA_NORMAL_BUFFER) {
        ctx.state.normalBuffer = ctx.bound_buffer;
    } else if (target == RUDA_TEXTURE_BUFFER) {
        ctx.state.texCoordBuffer = ctx.bound_buffer;
    } else if (target == RUDA_INDEX_BUFFER) {
        ctx.state.indexBuffer = ctx.bound_buffer;
    }
}


void rudaBufferData(Buffer_Target target, uint size, float* data, Buffer_Usage usage) {

    if (ctx.bound_buffer->buffer_pointer != nullptr) {
        // Delete the old buffer data
        if (RUDA_DEBUG) {
            std::cout << "rudaBufferData: Deleting/Freeing old buffered data." << std::endl;
        }
        if (CUDA_ENABLED) {
            //freeDeviceMemory(&ctx);
        } else {
            delete ctx.bound_buffer->buffer_pointer;
        }
     //throw std::invalid_argument("rudaBufferData: currently bound buffer is already mapped");   
    }
    
    ctx.bound_buffer_target = target;

    if (CUDA_ENABLED) {
        //cuda_bufferData(&ctx, (float*)data);
    } else {
        software_bufferData(&ctx, data, size);
    }
}
// These only really apply if the currently bound buffer object
// is an array_buffer_object
/**
void rudaVertexPointer(int size, int stride, const unsigned int pointer) {

    // Validate given parameters
    if (ctx.bound_buffer_target == RUDA_ARRAY_BUFFER) {
        // We want to keep track of the vertex pointers in the most portable
        // way possible
        // See the following diagram 
        //if (ctx.bound_vao) {
        //    ctx.bound_vao->vertex_data.size = size;
        //    ctx.bound_vao->vertex_data.stride = stride;
        //    ctx.bound_vao->vertex_data.start_offset = pointer;
        //} else {
        //    // apply attributes to immediate mode
        //    ctx.default_attributes.vertex_data.size = size;
        //    ctx.default_attributes.vertex_data.stride = stride;
        //    ctx.default_attributes.vertex_data.start_offset = pointer;
        //}
    }
    return;
}
void rudaColorPointer(int size, int stride, const unsigned int pointer) {
    if (ctx.bound_buffer_target == RUDA_ARRAY_BUFFER) {

        //if (ctx.bound_vao) {
        //    ctx.bound_vao->color_data.size = size;
        //    ctx.bound_vao->color_data.stride = stride;
        //    ctx.bound_vao->color_data.start_offset = pointer;
        //} else {
        //    // apply attributes to immediate mode
        //    ctx.default_attributes.color_data.size = size;
        //    ctx.default_attributes.color_data.stride = stride;
        //    ctx.default_attributes.color_data.start_offset = pointer;
        //}
    }
    return;
}
void rudaNormalPointer(int stride, const unsigned int pointer) {
    if (ctx.bound_buffer_target == RUDA_ARRAY_BUFFER) {

        //if (ctx.bound_vao) {
        //    ctx.bound_vao->normal_data.stride = stride;
        //    ctx.bound_vao->normal_data.start_offset = pointer;
        //} else {
        //    // apply attributes to immediate mode
        //    ctx.default_attributes.normal_data.stride = stride;
        //    ctx.default_attributes.normal_data.start_offset = pointer;
        //}
    }
    return;
}

void rudaTexCoordPointer(int size, int stride, const unsigned int pointer) {
    if (ctx.bound_buffer_target == RUDA_ARRAY_BUFFER) {

        //if (ctx.bound_vao) {
        //    ctx.bound_vao->tex_coord_data.stride = stride;
        //    ctx.bound_vao->tex_coord_data.start_offset = pointer;
        //} else {
        //    // apply attributes to immediate mode
        //    ctx.default_attributes.tex_coord_data.stride = stride;
        //    ctx.default_attributes.tex_coord_data.start_offset = pointer;
        //}
    }
    return;
}

void rudaIndexPointer(int stride, const unsigned int pointer) {
    if (ctx.bound_buffer_target == RUDA_ARRAY_BUFFER) {

        //if (ctx.bound_vao) {
        //    ctx.bound_vao->index_data.stride = stride;
        //    ctx.bound_vao->index_data.start_offset = pointer;
        //} else {
        //    // apply attributes to immediate mode
        //    ctx.default_attributes.index_data.stride = stride;
        //    ctx.default_attributes.index_data.start_offset = pointer;
        //}
    }
    return;
}
*/

void rudaDrawArrays(Draw_Mode mode, int count) {
    if (mode != RUDA_TRIANGLES) {
        std::cerr << "rudaDrawArrays: draw mode not yet supported." << std::endl;
        return;
    }

    if (CUDA_ENABLED) {
        
    } else {
        software_drawArrays(&ctx, RUDA_TRIANGLES, count);
    }
}

void ruda_test_print_bound_buffer(uint length) {
    for (int i = 0; i < length - 1; i++) {
        std::cout << ((float*)ctx.bound_buffer->buffer_pointer)[i] << ", ";
    }
    if (length > 0) {
        std::cout << ((float*)ctx.bound_buffer->buffer_pointer)[(int)length - 1] << std::endl;
    }
}

void rudaTempDrawToWindow(Display* display, Window window) {
    DrawFramebuffer(display, window, ctx.framebuffer, ctx.width, ctx.height);
	XSync(display, 0);
}

void rudaTransformMatrix(mat<4, 4>& mat) {
    ctx.state.model = mat;
}

void rudaTranslateMatrix(vec4 vecTranslate) {
    ctx.state.translate = vecTranslate;
}

void rudaProjectionMatrix(mat<4, 4>& mat) {
    ctx.state.projection = mat;
}

void rudaClearFB(float r, float g, float b) {
    if (CUDA_ENABLED) {
        
    } else {
        software_clearFB(&ctx, r, g, b);
    }
}