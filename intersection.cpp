//
//  intersection.cpp
//  RayTracer
//
//  Created by Uriana on 3/29/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#include "intersection.h"
intersection::intersection(){
    objectid = -1;
    Ph = Vector3D(0,0,0);
    nh = Vector3D(0,0,0);
    intersect = false;
    th = INFINITY;
    u = 0;
    v = 0;
    
}