#include "primitives.h"

std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    for (int i = 0; i < 3; ++i) {
        os << "Vertex " << i << ": (" << triangle.p[i][0] << ", " 
           << triangle.p[i][1] << ", " << triangle.p[i][2] << ")\n";
    }
    return os;
}