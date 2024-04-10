#include "../../src/ruda.h"
#include "../../src/rudautils.h"
#include <chrono>
#include <unistd.h>
#include <vector>
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

class Cube {

    mat<4, 4> rotationMatrix;
    vec<4> translationVector;
    public:
    float x;
    float y;
    float z;
    int scale;
    uint cube_vbo;

    Cube(float x, float y, float z, int scale, uint cube_vbo) : x(x), y(y), z(z), scale(scale), cube_vbo(cube_vbo) {
        translationVector = vec4{x, y, z, 0};
    };

    void rotate(float angleRad) {
        rotationMatrix = rotateX(angleRad) * rotateY(angleRad) * rotateZ(angleRad);
    };

    void draw() {
        rudaBindBuffer(RUDA_VERTEX_BUFFER, cube_vbo);
        rudaTranslateMatrix(translationVector);
        rudaTransformMatrix(rotationMatrix);
        rudaDrawArrays(RUDA_TRIANGLES, 36);
    };
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

    std::vector<Cube*> cubes;
    cubes.push_back(new Cube(0, 0, 50, 1, vertex_buffer));
    cubes.push_back(new Cube(-3, -3, 50, 1, vertex_buffer));
    cubes.push_back(new Cube(3, 3, 50, 1, vertex_buffer));

    mat<4, 4> perspective = genPerspective(500, 500);
    rudaProjectionMatrix(perspective);

    float angle = 0;

	while(true) {
        rudaClearFB();
        angle  += 0.01;
        for (Cube* cube : cubes) {
            cube->rotate(angle);
            cube->draw();
        }

		std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
		// 36 vertices in total, so we give 36 as count
		rudaTempDrawToWindow(display, window);
		//std::cout << 1000/std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " fps" << std::endl;
	}
}