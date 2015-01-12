//
//  intersection.h
//  RayTracer
//
//  Created by Uriana on 3/29/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__intersection__
#define __RayTracer__intersection__

#include <iostream>
#include "Vector3D.h"

class intersection{
public:
    intersection();
    Vector3D Ph;
    Vector3D nh;
    float u, v;
    bool intersect = false;
    float th;
    int objectid;
};
#endif /* defined(__RayTracer__intersection__) */
