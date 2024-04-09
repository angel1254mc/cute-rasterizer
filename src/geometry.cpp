#include "geometry.h"
// Cross product between two vec3 elements, where each vec3 element is a vector
// from the origin (useful for normals)
vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec<3>{v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}