//
//  objLoader.h
//  RayTracer
//
//  From http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/#Loading_the_OBJ
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__objLoader__
#define __RayTracer__objLoader__

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "Vector3D.h"

bool loadOBJ(
             const char * path,
             std::vector<Vector3D> & out_vertices,
             std::vector<Vector2D> & out_uvs,
             std::vector<Vector3D> & out_normals
             );

#endif /* defined(__RayTracer__objLoader__) */
