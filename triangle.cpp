//
//  triangle.cpp
//  RayTracer
//
//  Created by Uriana on 3/30/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#include "triangle.h"
triangle::triangle(Vector3D theP0, Vector3D theP1, Vector3D theP2, Vector3D then0, Vector3D then1, Vector3D then2)
{
    P0 = theP0;
    P1 = theP1;
    P2 = theP2;
    nAt0 = then0;
    nAt1 = then1;
    nAt2 = then2;
    
    n2 = ((nAt0 + nAt1 + nAt2)/3.0).Normalize();
    n0 = (P0 - P1).Normalize();
    n1 = Cross(n2, n0).Normalize();

}

Vector3D triangle::getP0(void){
    return P0;
}
Vector3D triangle::getP1(void){
    return P1;
}
Vector3D triangle::getP2(void){
    return P2;
}

Vector3D triangle::getn0(void){
    return n0;
}
Vector3D triangle::getn1(void){
    return n1;
}
Vector3D triangle::getn2(void){
    return n2;
}

Vector3D triangle::getnAt0(void){
    return nAt0;
}
Vector3D triangle::getnAt1(void){
    return nAt1;
}
Vector3D triangle::getnAt2(void){
    return nAt2;
}


int triangle::objectType(){
    return 2;
}
