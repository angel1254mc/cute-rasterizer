#pragma once
#include "geometry.h"
#include <cmath>

mat<4, 4> rotateX(float angle) {
    mat<4, 4> rotationX;
    rotationX[0] = {1, 0, 0, 0};
    rotationX[1] = {0, cosf(angle), -1 * sinf(angle), 0};
    rotationX[2] = {0, sinf(angle), cosf(angle), 0};
    rotationX[3] = {0, 0, 0, 1};

    return rotationX;
}

mat<4, 4> rotateY(float angle) {
    mat<4, 4> rotationX;
    rotationX[0] = {cosf(angle), 0, sinf(angle), 0};
    rotationX[1] = {0, 1, 0, 0};
    rotationX[2] = {-1 * sinf(angle), 0, cosf(angle), 0};
    rotationX[3] = {0, 0, 0, 1};

    return rotationX;
}


mat<4, 4> rotateZ(float angle) {
    mat<4, 4> rotationX;
    rotationX[0] = {cosf(angle), -1 * sinf(angle), 0, 0};
    rotationX[1] = {sinf(angle), cosf(angle), 0, 0};
    rotationX[2] = {0, 0, 1, 0};
    rotationX[3] = {0, 0, 0, 1};

    return rotationX;
}

mat<4, 4> genOrtho(float left, float right, float bottom, float top, float near, float far) {
    mat<4, 4> orthoMatrix;
    orthoMatrix[0] = { 2/ (right - 1), 0, 0, 0};
    orthoMatrix[1] = {0, 2 / (top - bottom), 0, 0};
    orthoMatrix[2] = {0, 0, -2/(far - near), 0};
    orthoMatrix[3] = {-(right + left) / (right - 1), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1};
    
    return orthoMatrix;
}

mat<4, 4> genPerspective(int w, int h, float fNear = 0.1f, float fFar = 1000.0f, float fFov = 90.0f) {

    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fAspectRatio = (float)h/ (float) w;

    // This is going to be the projection matrix
    // I.E. takes our world and flattens it.
    mat<4, 4> projection;
    projection[0][0] = fAspectRatio * fFovRad;
    projection[1][1] = fFovRad;
    projection[2][2] = fFar / (fFar - fNear);
    projection[3][2] = (-fFar * fNear) / (fFar - fNear);
    projection[2][3] = 1.0f;
    projection[3][3] = 0.0f;

    return projection;
}