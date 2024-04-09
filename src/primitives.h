#pragma once
#include <vector>
#include <math.h>
#include "geometry.h"
#include "context.h"

class Vertex {
    public:
    vec3 color;
    vec3 pos;
    vec3 normal;
    vec2 uv;

    Vertex(float x, float y, float z) {
        pos = vec3{x, y, z};
    }
};

class Triangle {
    public:
    // Three (x, y, z) points make a triangle
    // mutable
    mat<3,3> p;
    // Stores vertex properties
    // should not be changed!
    std::vector<Vertex*> pObj;
    //  Normal, either provided by normal buffer or calculated on the fly
    vec3 normal; 
    // optional normal calculation
    void calcNormal() {
        vec3 line1, line2;
        line1 = p[1] - p[0];
        line2 = p[2] - p[0];
        normal = cross(line1, line2);
        normal = normal/normal.norm2();
    };

    Triangle(Vertex* v1, Vertex* v2, Vertex* v3) {
        // Push back the vertex object
        pObj.push_back(v1);
        pObj.push_back(v2);
        pObj.push_back(v3);
        // Fill the triangle matrix with vertex coords.
        p[0] = v1->pos;
        p[1] = v2->pos;
        p[2] = v3->pos;        
    }
};

std::ostream& operator<<(std::ostream& os, const Triangle& triangle);

