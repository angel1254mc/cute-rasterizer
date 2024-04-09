// main.cpp
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <strstream>
#include "mesh.h"
using namespace std;


void clearScreen(Display *display, Window window) {
    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);

    XSetForeground(display, DefaultGC(display, DefaultScreen(display)), BlackPixel(display, DefaultScreen(display)));
    XFillRectangle(display, window, DefaultGC(display, DefaultScreen(display)), 0, 0, attr.width, attr.height);
}

void drawTriangle(Display *display, Window window, int x1, int y1, int x2, int y2, int x3, int y3) {
    GC gc = DefaultGC(display, DefaultScreen(display));
    unsigned long white = WhitePixel(display, DefaultScreen(display));
    XSetForeground(display, gc, white);
    XPoint points[4];
    points[0].x = x1;
    points[0].y = y1;
    points[1].x = x2;
    points[1].y = y2;
    points[2].x = x3;
    points[2].y = y3;
    points[3] = points[0];  // Connect back to the starting point to close the triangle

    XDrawLines(display, window, gc, points, 4, CoordModeOrigin);
}


Mesh meshCube = Mesh();

int main() {

    meshCube.tris = {

		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	};


    int screenHeight = 600;
    int screenWidth = 800;
    // Projection Matrix
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;

    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fAspectRatio = (float)screenHeight / (float) screenWidth;

    // This is going to be the projection matrix
    // I.E. takes our world and flattens it.
    mat4 projection;
    projection.m[0][0] = fAspectRatio * fFovRad;
    projection.m[1][1] = fFovRad;
    projection.m[2][2] = fFar / (fFar - fNear);
    projection.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    projection.m[2][3] = 1.0f;
    projection.m[3][3] = 0.0f;


    Display *display;
    Window window;
    XEvent event;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        cout << "No Shot" << endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 800, 600, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose)
            break;
    };

    int time = 1;
    while(true) {
        clearScreen(display, window);
        time++;

        mat4 rotateZMatrix = rotateZ(1.0f * time / 100);
        mat4 rotateXMatrix = rotateX(1.0f * time / 100);
        mat4 rotateYMatrix = rotateY(1.0f * time / 100);

        for (auto tri: meshCube.tris) {
            
        }

        for (auto tri : meshCube.tris) {
            Triangle triProjected, triTranslated, triRotatedXYZ, triRotatedXZ, triRotatedX;
            MultiplyMatrices(tri, triRotatedX, rotateXMatrix);
            MultiplyMatrices(triRotatedX, triRotatedXZ, rotateZMatrix);
            MultiplyMatrices(triRotatedXZ, triRotatedXYZ, rotateYMatrix);

            triTranslated = translate(triRotatedXYZ, 0, 0, 3.0f);
            Vec3d cameraPos = Vec3d{0, 0, 0};
            triTranslated.calcNormal();
            Vec3d camRay = (triTranslated.p[0] - cameraPos);
            if ((triTranslated.normal.dot(camRay)) <= 0.0f)
                continue;

            MultiplyMatrices(triTranslated, triProjected, projection);

			// Scale into view
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
			triProjected.p[0].x *= 0.5f * (float)screenWidth;
			triProjected.p[0].y *= 0.5f * (float)screenHeight;
			triProjected.p[1].x *= 0.5f * (float)screenWidth;
			triProjected.p[1].y *= 0.5f * (float)screenHeight;
			triProjected.p[2].x *= 0.5f * (float)screenWidth;
			triProjected.p[2].y *= 0.5f * (float)screenHeight;

            drawTriangle(display, 
                        window, 
                        triProjected.p[0].x, triProjected.p[0].y,
                        triProjected.p[1].x, triProjected.p[1].y,
                        triProjected.p[2].x, triProjected.p[2].y);
        }   
        XSync(display, 0);
        usleep(10000);
    }

    XCloseDisplay(display);
    return 0;
}
