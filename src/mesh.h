#pragma once
#include <vector>
#include "primitives.h"
#include <string>
#include <fstream>
#include <strstream>
using namespace std;

class Mesh { 
    public:
    vector<Triangle> tris;
    bool LoadFromObjectFile(string sFilename);
};