//
//  texture.h
//  RayTracer
//
//  Created by Uriana on 4/2/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__texture__
#define __RayTracer__texture__

#include <iostream>
#include "ppmview.h"
#include "Vector3D.h"

class texture{
public:
    void loadImage(char * filename);
    Point3D getColorAt(float u, float v);
    bool hasTex = false;
    int width, height;

private:
    ppmview textureImage;
    unsigned char *pixmap;
};
#endif /* defined(__RayTracer__texture__) */
