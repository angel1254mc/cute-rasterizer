#pragma once
#include <vector>
#include "context.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// Takes
void rudaGenBuffers(uint n, uint *buffers);

void rudaDeleteBuffers(uint n, uint* buffers);

void rudaBindBuffer(Buffer_Target target, uint buffer);

void rudaBufferData(Buffer_Target target, uint size, float* data, Buffer_Usage usage);

// Specifying fields/properties in a bound buffer object
//void rudaVertexPointer(int size, int stride, const unsigned int pointer);
//void rudaColorPointer(int size, int stride, const unsigned int pointer);
//void rudaNormalPointer(int stride, const unsigned int pointer);
//void rudaTexCoordPointer(int size, int stride, const unsigned int pointer);
//void rudaIndexPointer(int stride, const unsigned int pointer);

// Immediate Mode Color/Vertex Specifications

/**
 * @param mode Denotes the type of primitive to render, can be any of Draw_Mode enums
 * @param count Denotes the total count of vertices, where each vertex is expected to be a float
*/
void rudaDrawArrays(Draw_Mode mode, int count);

/**
 * @brief Clears the current framebuffer, with the option to provide a color to fill the fb with
 * @note by default, rudaClearFB sets all pixels in the fb to black
*/
void rudaClearFB(float r = 0, float g = 0, float b = 0);

void rudaProjectionMatrix(mat<4, 4>& mat);
void rudaTransformMatrix(mat<4, 4>& mat);
void rudaTranslateMatrix(vec4 vecTranslate);
void ruda_test_print_bound_buffer(uint length);
void rudaTempDrawToWindow(Display* display, Window window);

void rudaGenTextures(uint n, uint* textures);
void rudaDeleteTextures(uint n, uint *buffers);
void rudaBindTexture(Ruda_Texture_Target target, uint texture);
void rudaTexImage2D(Ruda_Texture_Target target, Texture_Format format, uint width, uint height, const void * data);