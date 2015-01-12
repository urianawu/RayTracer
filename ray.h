//
//  ray.h
//  RayTracer
//
//  Created by Uriana on 3/29/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__ray__
#define __RayTracer__ray__

#include <iostream>
#include "geometry.h"
#include "intersection.h"
#include "scene.h"
#include "camera.h"
#include <vector>
#define pi 3.14159265358

class ray{
public:
    ray(Vector3D Pr, Vector3D nr);
    std::vector<intersection> tracingMultipleObjectGetAll(std::vector<geometry*>);
    intersection tracingMultipleObject(std::vector <geometry*>);
    intersection tracing(geometry *);
    Vector3D getPr();
    Vector3D getnr();
    void setBounces(int bounces);
    int getBounces();
private:

    Vector3D Pr;
    Vector3D nr;
    int bounces = 2;
};
#endif /* defined(__RayTracer__ray__) */
