#pragma once
#include "../context.h"
#include "../ruda.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <limits>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../primitives.h"

void software_bufferData(Ruda_Context* ctx, float* data, uint size) {
    if (RUDA_DEBUG)
            std::cout << "rudaBufferData: Copying buffer over to buffer_map_pointer" << std::endl;
	// Create internal buffer and copy data
    ctx->bound_buffer->buffer_pointer = new float[size/(sizeof(float))];  
    std::memcpy(ctx->bound_buffer->buffer_pointer, data, size);
	// Initialize buffer data
	ctx->bound_buffer->length = size / sizeof(float);
	
    if (RUDA_DEBUG)
        std::cout << "rudaBufferData: Finished copying buffer" << std::endl;
}

//bool validateDrawArrays(Ruda_Context* ctx, Draw_Mode mode, int count) {
//	// Before we begin drawing, we should check
//	// 1. There is stuff to draw
//	// 	Verify that there is a bound buffer
//	// 	Otherwise throw an error. We expect some buffer to be available for us to use
//	// 2. Check that the stuff has been supplied in a valid way
//	// 	  If we have a bound buffer, that buffer is either a
//	//		a. RUDA_ARRAY_BUFFER - i.e. contains attributes to denote
//	//		things like vertices, colors, etc.
//	//				IF we have an ARRAY_BUFFER, we expect attributes to be supplied as well
//	//		b. RUDA_VERTEX_BUFFER, RUDA_COLOR_BUFFER, RUDA_NORMAL_BUFFER, etc.	
//
//	assert(ctx->bound_buffer);
//	assert(ctx->bound_buffer_target);
//	
//	if (ctx->bound_buffer_target == RUDA_VERTEX_BUFFER ||
//		ctx->bound_buffer_target == RUDA_COLOR_BUFFER ||
//		ctx->bound_buffer_target == RUDA_NORMAL_BUFFER ||
//		ctx->bound_buffer_target == RUDA_TEXTURE_BUFFER ||
//		ctx->bound_buffer_target == RUDA_INDEX_BUFFER) {
//			
//			// vertex buffer needs to exist, otherwise nothing can be drawn!
//			assert(ctx->state.vertexBuffer != nullptr);
//
//			// If we are drawing triangles, verify that
//			// buffer vertices are divisible by 3
//			// count of vertices we want to draw is also divisible by 3
//			if (mode == RUDA_TRIANGLES) {
//				assert((ctx->state.vertexBuffer->size/sizeof(float) % 3 == 0));
//				assert(count % 3 == 0);
//			} else  {
//				std::cerr << "rudaDrawArrays: given draw mode not supported yet." << std::endl;
//				return false;
//			}
//
//
//			assert(ctx->state.vertexBuffer->size <= sizeof(float) * count);
//			if (ctx->state.colorBuffer) {
//				assert(ctx->state.colorBuffer->size <= sizeof(float) * count);
//			}
//			if (ctx->state.normalBuffer) {
//				// Consider the case where normals are provided per-face
//				if (ctx->state.normalBuffer->size < ctx->state.vertexBuffer->size)
//					assert(ctx->state.normalBuffer->size <= (sizeof(float) * count / 3));
//			}
//		
//	} else if (ctx->bound_buffer_target == RUDA_ARRAY_BUFFER) {
//		// Do a whole bunch of other stuff
//		std::cerr << "validateDrawArrays: RUDA_ARRAY_BUFFER not yet supported!" << std::endl;
//		return false;
//	} else {
//		 std::cerr << "Invalid buffer target provided, expected RUDA_ARRAY_BUFFER mode or other RUDA_*_BUFFER mode" << std::endl;
//		 return false;
//	}
//}

void assemblePrimitives(Ruda_Context* ctx, std::vector<Triangle*>& tris, int count) {
	// Take all the vertices in the current RUDA_VERTEX_BUFFER
	// up to count of course
	// Construct triangles and apply ModelView Transformations
	float* buffer_pointer, *color_buffer_pointer, *normal_buffer_pointer;
	buffer_pointer = nullptr;
	color_buffer_pointer = nullptr;
	normal_buffer_pointer = nullptr;

	if (ctx->state.vertexBuffer)
		buffer_pointer = (float*)(ctx->state.vertexBuffer->buffer_pointer);
	if (ctx->state.colorBuffer)
		color_buffer_pointer = (float*)(ctx->state.colorBuffer->buffer_pointer);
	if (ctx->state.normalBuffer)
		normal_buffer_pointer = (float*)(ctx->state.normalBuffer->buffer_pointer);

	std::vector<Vertex*> currTriangle;
	for (int i = 0; i < count * 3; i += 3) {
		currTriangle.push_back(
			new Vertex(buffer_pointer[i], buffer_pointer[i + 1], buffer_pointer[i + 2])
		);
		if (color_buffer_pointer)
			currTriangle.back()->color = vec3{color_buffer_pointer[i], color_buffer_pointer[i + 1], color_buffer_pointer[i + 2]};
		if (normal_buffer_pointer)
			currTriangle.back()->normal = vec3{normal_buffer_pointer[i], normal_buffer_pointer[i + 1], normal_buffer_pointer[i + 2]};
		if (currTriangle.size() == 3) {
			tris.push_back(new Triangle(currTriangle[0], currTriangle[1], currTriangle[2]));
			// Clear for next triangle to be built
			currTriangle.clear();
		}
	};

	
};

vec<2> screen_space(Ruda_Context* ctx, float x, float y) {
	return vec2(int((((x + 1) * ctx->width) * 0.5f) + 0.5f), int((((y + 1) * ctx->height) * 0.5f) + 0.5f));
};

inline float edge_function(const vec2& v1, const vec2& v2, const vec2& v3) {
	return (v2.x - v1.x)*(v3.y - v1.y) - (v2.y - v1.y)*(v3.x - v1.x);
}

vec<4> getBoundingBox(Ruda_Context* ctx, vec2& v1, vec2& v2, vec2& v3) {
	vec4 bb;
	vec2 clamp = {ctx->width - 1, ctx->height - 1};
	// min x, y
	bb[0] = std::numeric_limits<float>::max();
	bb[1] = std::numeric_limits<float>::max();
	// max x, y
	bb[2] = -std::numeric_limits<float>::max();
	bb[3] = -std::numeric_limits<float>::max();

	bb[0] = std::max(0.0f, std::min(bb[0], v1.x));
	bb[0] = std::max(0.0f, std::min(bb[0], v2.x));
	bb[0] = std::max(0.0f, std::min(bb[0], v3.x));

	bb[1] = std::max(0.0f, std::min(bb[1], v1.y));
	bb[1] = std::max(0.0f, std::min(bb[1], v2.y));
	bb[1] = std::max(0.0f, std::min(bb[1], v3.y));

	bb[2] = std::min(clamp.x, std::max(bb[2], v1.x));
	bb[2] = std::min(clamp.x, std::max(bb[2], v2.x));
	bb[2] = std::min(clamp.x, std::max(bb[2], v3.x));

	bb[3] = std::min(clamp.y, std::max(bb[3], v1.y));
	bb[3] = std::min(clamp.y, std::max(bb[3], v2.y));
	bb[3] = std::min(clamp.y, std::max(bb[3], v3.y));

	return bb;
}

void rasterize(Ruda_Context* ctx, Triangle* tri) { 
	// Multiply with MVP
	vec4 v1NDC = ctx->state.projection * (ctx->state.model* embed<4>(tri->p[0]) + ctx->state.translate);
	vec4 v2NDC = ctx->state.projection * (ctx->state.model* embed<4>(tri->p[1]) + ctx->state.translate);
	vec4 v3NDC = ctx->state.projection * (ctx->state.model* embed<4>(tri->p[2]) + ctx->state.translate);

	Vertex* v1Obj = tri->pObj[0];
	Vertex* v2Obj = tri->pObj[1];
	Vertex* v3Obj = tri->pObj[2];

	float v1_view_z = v1NDC[3];
	float v2_view_z = v2NDC[3];
	float v3_view_z = v3NDC[3];
	
	// Perform perspective division
	v1NDC = v1NDC / v1NDC[3];
	v2NDC = v2NDC / v2NDC[3];
	v3NDC = v3NDC / v3NDC[3];

	// We should have our properly projected
	// NDC coords now, all that's left is to convert to screen coords
	vec2 v1_screen = screen_space(ctx, v1NDC[0], v1NDC[1]);
	vec2 v2_screen = screen_space(ctx, v2NDC[0], v2NDC[1]);
	vec2 v3_screen = screen_space(ctx, v3NDC[0], v3NDC[1]);

	if (RUDA_DEBUG) {
		std::cout << "Triangle Screen Space Coordinates:" << std::endl;
		std::cout << v1_screen << std:: endl;
		std::cout << v2_screen << std::endl;
		std::cout << v3_screen << std::endl;
	}


	// bounding_box[0,1] = minx, miny
	// bounding_box[2,3] = maxx, maxy
	vec4 bounding_box = getBoundingBox(ctx, v1_screen, v2_screen, v3_screen);
	// Divide vertex attributes by view space Z for perspective correct interpolation
    //vec3 v0n = tri->pObj[0]->normal / v1_view_z;
    //vec3 v1n = tri->pObj[1]->normal / v2_view_z;
    //vec3 v2n = tri->pObj[2]->normal  / v3_view_z;

	// Code from scratchapixel.com 
	float area = edge_function(v1_screen, v2_screen, v3_screen);
	vec2 p;

	for (p.x = bounding_box[0]; p.x <= bounding_box[2]; p.x++)
		for (p.y = bounding_box[1]; p.y <= bounding_box[3]; p.y++) {
			// barycentric coordinates please
			float w0 = edge_function(v2_screen, v3_screen, p);
			float w1 = edge_function(v3_screen, v1_screen, p);
			float w2 = edge_function(v1_screen, v2_screen, p);

			// if all 3 are at or above 0, the given pixel is inside the triangle
			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				w0 /= area;
				w1 /= area;
				w2 /= area;

				// Get interpolated pixel depth and check whether depth test passes
				float z = 1.0f / (v1_view_z * w0 + v2_view_z * w1 + v3_view_z * w2);
				if (z < ctx->depth_buffer[int(p.y * ctx->height + p.x)]) {
					// New minimum depth just dropped
					ctx->depth_buffer[int(p.y * ctx->width + p.x)] = z;

					// @todo angel1254mc
					// Insert texture interpolation here if we are using textures, otherwise use colors
					float r = w0 * v1Obj->color[0] + w1 * v2Obj->color[0] + w2 * v3Obj->color[0];
					float g = w0 * v1Obj->color[1] + w1 * v2Obj->color[1] + w2 * v3Obj->color[1];
					float b = w0 * v1Obj->color[2] + w1 * v2Obj->color[2] + w2 * v3Obj->color[2];

					ctx->framebuffer[int(3*(p.x + p.y * ctx->width) + 0)] = r;
					ctx->framebuffer[int(3*(p.x + p.y * ctx->width) + 1)] = g;
					ctx->framebuffer[int(3*(p.x + p.y * ctx->width) + 2)] = b;
					
				}
			}
		}

}


void DrawFramebuffer(Display* display, Window window, float* framebuffer, int width, int height) {
	if (!ctx.gc)
		ctx.gc = XCreateGC(display, window, 0, 0);
	char *image32=(char *)malloc(width*height*4);
    char *p=image32;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            *p++ = static_cast<int>(framebuffer[(y * width + x) * 3] * 255);
            *p++ = static_cast<int>(framebuffer[(y * width + x) * 3 + 1] * 255);
            *p++ = static_cast<int>(framebuffer[(y * width + x) * 3 + 2] * 255);
			*p++ = 1;
        }
    }
	XImage* ximage = XCreateImage(display, DefaultVisual(display, 0), DefaultDepth(display, DefaultScreen(display)),
    ZPixmap, 0, image32, width, height, 32, 0);
	XPutImage(display, window, ctx.gc, ximage,
                    0, 0, 0, 0, width, height);
	XDestroyImage(ximage);
}



void ruda_debug_display_pixels(float* framebuffer, int width, int height) {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Unable to open X display" << std::endl;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        0, 0, width, height, 1,
                                        BlackPixel(display, screen),
                                        WhitePixel(display, screen));


    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void software_drawArrays(Ruda_Context* ctx, Draw_Mode mode,  int count) {
    if (!ctx->state.vertexBuffer || !ctx->state.colorBuffer || count <= 0) {
        std::cerr << "Invalid vertex buffer, color buffer, or size!" << std::endl;
        return;
    };
	// If setup is not valid, blow up
	//if (!validateDrawArrays(ctx, mode, size))
	//	return;

	// Reset depth buffer before triangles are rasterized
	for (int i = 0; i < ctx->width * ctx->height; i++)
		ctx->depth_buffer[i] = std::numeric_limits<float>::max();


	// Commented out because this occurs on rudaClearFB
	//for (int i =0; i < ctx->width * ctx->height * 3; i++)
	//	ctx->framebuffer[i] = 0;
	

	std::vector<Triangle*> tris;
	assemblePrimitives(ctx, tris, count);

	//if (RUDA_DEBUG) {
	//	std::cout << "software_drawArrays: Printing Triangle Data..." << std::endl;
	//	int i = 0;
	//	for (Triangle* tri : tris) {
	//		std::cout << i << "th Triangle: " << std::endl;
	//		std::cout << (*tri) << std:: endl;
	//		i++;
	//	}
	//};

	//  cache modelview matrix for vertx transformations
	ctx->state.mv = ctx->state.view * ctx->state.model;
	int i = 0;
	for (Triangle* tri : tris) {
		rasterize(ctx,tri);
		i++;
	}

	//if (RUDA_DEBUG)
		//ruda_debug_display_pixels(ctx->framebuffer, ctx->width, ctx->height);

}

void software_clearFB(Ruda_Context* ctx,  float r = 0, float g = 0, float b = 0)  {
	for (int i =0; i < ctx->height; i++)
		for (int j = 0; j < ctx->width; j++) {
			ctx->framebuffer[(i * ctx->height  + j)] = r;
			ctx->framebuffer[(i * ctx->height  + j + 1)] = g;
			ctx->framebuffer[(i * ctx->height  + j + 2)] = b;
		};
}