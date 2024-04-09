#pragma once
#include <vector>
#include "geometry.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define CUDA_ENABLED false
#define RUDA_DEBUG false

typedef unsigned int uint;


enum Buffer_Usage {
    STREAM_DRAW, 
    STREAM_READ,
    STREAM_COPY, 
    STATIC_DRAW,
    STATIC_READ, 
    STATIC_COPY,
    DYNAMIC_DRAW, 
    DYNAMIC_READ,
    DYNAMIC_COPY
};

enum Draw_Mode {
    RUDA_TRIANGLES,
    RUDA_TRIANGLE_STRIP,
    RUDA_POINTS,
    RUDA_LINE_STRIP,
    RUDA_LINES
};

enum Buffer_Target {
    //  General Array buffer, traditional VBO
    // Attributes modified with calls to rudaAttribPointer
    RUDA_ARRAY_BUFFER, 
    RUDA_VERTEX_BUFFER, // Vertex-specific buffer
    RUDA_COLOR_BUFFER, // color-specific buffer
    RUDA_NORMAL_BUFFER, // normal-specific buffer
    RUDA_INDEX_BUFFER, // index-specific buffer
    RUDA_TEXTURE_BUFFER, // tex coord specific buffer
    //RUDA_ELEMENT_ARRAY_BUFFER,
    //RUDA_TEXTURE_BUFFER,
    //RUDA_UNIFORM_BUFFER
};

class Buffer_State {
    public: 

    Buffer_Usage usage;
    Buffer_Target bound_buffer_target;

    //  Actual raw array data
    int length;
    float* buffer_pointer;
    bool is_bound = false;
    
    Buffer_State() {
        buffer_pointer = nullptr;
    };

    ~Buffer_State();
};


// #### POINTER DATA FOR BOUND ARRAY BUFFERS
class Vertex_Pointer {
    public:
    bool enabled = false;
    int stride = 0;
    int size = 3;
    int start_offset = 0;
};
class Color_Pointer {
    public:
    bool enabled = false;
    int stride = 0;
    int size = 3;
    int start_offset = 0;
};
class Normal_Pointer {
    int size = 3;
    public:
    bool enabled = false;
    int stride = 0;
    int start_offset = 0;
};
class Texture_Pointer {
    int size = 2;
    public:
    bool enabled = false;
    int stride = 0;
    int start_offset = 0;
};
class Index_Pointer  {
    int size = 1;
    public:
    bool enabled = false;
    int stride = 0;
    int start_offset = 0;
};

class Buffer_Attributes {
    public:
    Vertex_Pointer vertex_data;
    Color_Pointer color_data;
    Normal_Pointer normal_data;
    Texture_Pointer tex_coord_data;
    Index_Pointer index_data;
};

class Immediate_Attributes {
    vec4 color;
    vec3 normal;
    vec3 tex_coord;
    int index;
};

class Ruda_State {
    public:
    // Keep track of Immediate-ish mode buffers 
    // (RUDA_ARRAY_BUFFER is not included here, as it defines its own
    // storage mechanism for vertices, colors, normals, etc.)
    Buffer_State* vertexBuffer = nullptr;
    Buffer_State* colorBuffer = nullptr;
    Buffer_State* normalBuffer = nullptr;
    Buffer_State* texCoordBuffer = nullptr;
    Buffer_State* indexBuffer = nullptr;
    mat<4, 4> model = mat<4,4>().identity();
    vec4 translate = vec4{0, 0, 0, 0};
    mat<4, 4> view = mat<4,4>().identity();
    // Model * View, cached prior to triangle rasterization;
    mat<4, 4> mv;
    mat<4, 4> projection = mat<4,4>().identity();
};

class Ruda_Context {
    public:
    std::vector<Buffer_State*> buffers;
    //std::vector<Buffer_Attributes*> vaos;
    Ruda_State state;
    Buffer_State* bound_buffer = nullptr;
    //Buffer_Attributes* bound_vao = nullptr;
    Buffer_Target bound_buffer_target;
    //Buffer_Attributes default_attributes;
    Buffer_Usage usage;
    
    // Begin-End mechanism won't be implemented for now
    //bool inBeginEnd = false;
    //Immediate_Attributes immediate_attributes;


    unsigned int width = 500;
    unsigned int height = 500;
    float* framebuffer = new float[width*height*3];
    float* depth_buffer = new float[width*height];

    GC gc = NULL;
    
    
};

extern Ruda_Context ctx;

