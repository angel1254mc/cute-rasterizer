#include "../src/context.h"
#include "../src/ruda.h"
#include <iostream>
uint vertex_buffer; // Save this for later rendering

int main()
{
    rudaGenBuffers(1, &vertex_buffer);
    std::cout << "vertex_buffer reserved name: " << vertex_buffer << std::endl;
    rudaBindBuffer(RUDA_ARRAY_BUFFER, vertex_buffer);

    float vbo_data[] = {
        -1.0, 0.0, 1.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };

    std::cout << "Buffer successfully bound" << std::endl;
    std::cout << "Buffer Size: " << sizeof(vbo_data) << std::endl;

    rudaBufferData(RUDA_VERTEX_BUFFER, sizeof(vbo_data), vbo_data, STATIC_DRAW);

    
    std::cout << "Buffer Data Loaded" << std::endl;

    ruda_test_print_bound_buffer(9);

    rudaDeleteBuffers(1, &vertex_buffer);
}
