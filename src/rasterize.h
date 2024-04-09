#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <strstream>
#include "mesh.h"


// Handles all stages of the rasterization pipeline
// For each object in our scene, 
// 1. Assemble the primitive associated with that object
// 2. Transform the primitive MVP + clipping
// 3. 
void rasterize(vector<Mesh> objects) {

};