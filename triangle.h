//
//  triangle.h
//  RayTracer
//
//  Created by Uriana on 3/30/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__triangle__
#define __RayTracer__triangle__

#include <iostream>
#include "geometry.h"

class triangle: public geometry
{
public:
    triangle(Vector3D theP0, Vector3D theP1, Vector3D theP2, Vector3D then0, Vector3D then1, Vector3D then2);
    Vector3D getP0(void);
    Vector3D getP1(void);
    Vector3D getP2(void);
    
    Vector3D getn0(void);
    Vector3D getn1(void);
    Vector3D getn2(void);
    
    Vector3D getnAt0(void);
    Vector3D getnAt1(void);
    Vector3D getnAt2(void);
    
    int objectType();
private:
    Vector3D P0;
    Vector3D P1;
    Vector3D P2;
    Vector3D n0, n1, n2;
    Vector3D nAt0, nAt1, nAt2;
    
};
#endif /* defined(__RayTracer__triangle__) */
