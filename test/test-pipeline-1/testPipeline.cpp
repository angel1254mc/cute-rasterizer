#include "../../src/ruda.h"
#include "../../src/rudautils.h"
#include <chrono>
#include<unistd.h>
float vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


float colors[] = {
    // Front face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 0.0f, 0.0f, // Red
    0.0f, 0.0f, 1.0f, // Blue
    1.0f, 1.0f, 0.0f, // Yellow

    // Back face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 1.0f, 0.0f, // Yellow
    0.0f, 0.0f, 1.0f, // Blue
    1.0f, 1.0f, 0.0f, // Yellow

    // Top face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 1.0f, 0.0f, // Yellow
    0.0f, 0.0f, 1.0f, // Blue
    1.0f, 1.0f, 0.0f, // Yellow

    // Bottom face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 1.0f, 0.0f, // Yellow
    0.0f, 1.0f, 0.0f, // Green
    1.0f, 1.0f, 0.0f, // Yellow

    // Right face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 1.0f, 0.0f, // Yellow
    0.0f, 0.0f, 1.0f, // Blue
    1.0f, 1.0f, 0.0f, // Yellow

    // Left face
    1.0f, 0.0f, 0.0f, // Red
    0.0f, 1.0f, 0.0f, // Green
    0.0f, 0.0f, 1.0f, // Blue

    1.0f, 1.0f, 0.0f, // Yellow
    0.0f, 1.0f, 0.0f, // Green
    1.0f, 1.0f, 0.0f  // Yellow
};



int main() {

	Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Unable to open X display" << std::endl;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        0, 0, 500, 500, 1,
                                        BlackPixel(display, screen),
                                        WhitePixel(display, screen));

	XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (true) {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == Expose) {
            break;
        }
        else if (event.type == KeyPress) {
            break;
        }
    }

	uint vertex_buffer;
	uint color_buffer;

	rudaGenBuffers(1, &vertex_buffer);
	rudaGenBuffers(1, &color_buffer);
	
	rudaBindBuffer(RUDA_VERTEX_BUFFER, vertex_buffer);
    rudaBufferData(RUDA_VERTEX_BUFFER, sizeof(vertices), vertices, STATIC_DRAW);
  
    rudaBindBuffer(RUDA_COLOR_BUFFER, color_buffer);
    rudaBufferData(RUDA_COLOR_BUFFER, sizeof(colors), colors, STATIC_DRAW);
    // if no model (transformation) matrix, identity matrix
    // if no view matrix (world-to-view), slap identity matrix too
    // if no projection matrix, we once again have the identity matrix
    mat<4, 4> perspective = genPerspective(500, 500);
    rudaProjectionMatrix(perspective);
    float angle = 0;
	while(true) {
		mat<4, 4> transform = (rotateX(angle) * rotateZ(angle));
		vec4 translation;
        translation[2] = 100;
		angle += 0.01;
		std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

		// Set the transform matrix
        rudaTranslateMatrix(translation);
		rudaTransformMatrix(transform);
		rudaDrawArrays(RUDA_TRIANGLES, 36);

		std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
		// 36 vertices in total, so we give 36 as count
		rudaTempDrawToWindow(display, window);
		std::cout << 1000/std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " fps" << std::endl;
	}
}