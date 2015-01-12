//
//  ray.cpp
//  RayTracer
//
//  Created by Uriana on 3/29/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#include "ray.h"

ray::ray(Vector3D Pr, Vector3D nr){
    this->Pr = Pr;
    this->nr = nr;
}

Vector3D ray::getPr(){
    return Pr;
}

Vector3D ray::getnr(){
    return nr;
}

void ray::setBounces(int bounces)
{
    this->bounces = bounces;
}
int ray::getBounces(){
    return bounces;
}

std::vector<intersection> ray::tracingMultipleObjectGetAll(std::vector<geometry*> objects){
    int size = (int)objects.size();
    std::vector<intersection> in;
    
            for (int i = 0; i < size; i++) {
            intersection tmp = tracing(objects[i]);
            if (tmp.th < INFINITY) {
                tmp.objectid = i;
                in.push_back(tmp);
                
            }
        }
    return in;
}

intersection ray::tracingMultipleObject(std::vector <geometry*> objects)
{
    int objectID;
    int size = (int)objects.size();
    intersection in;
    in = tracing(objects[0]);
    objectID = 0;
    
    if(size > 1){
        for (int i = 1; i < size; i++) {
            if (objects[i]->getShader()->getTransparency() == 1) {
                intersection tmp = tracing(objects[i]);
                if (tmp.th < in.th) {
                    in = tmp;
                    objectID = i;
                }
            }
        }
        
    }
    in.objectid = objectID;
    return in;
    
}

intersection ray::tracing(geometry *object)
{
    intersection i;
    
    switch(object->objectType())
    {
        case 0:
        {
            //plane
            //printf("%f\t",Dot(object->getn0(),nr));
            if (Dot(object->getn2(),nr)!= 0) {
                i.th = ( Dot(object->getn2(),object->getPc()) - Dot(object->getn2(),Pr)) / Dot(object->getn2(),nr);
                if (i.th > 0.0 ){
                    //intersection occurs
                    i.Ph = Pr + nr * i.th;
                    i.nh = object->getn2();
                    i.intersect = true;
                    //textures
                    Point3D P00;
                    P00 = object->getPc() - object->gets0()/2 *object->getn0() - object->gets1()/2 * object->getn1();

                    i.u = Dot(object->getn0(), (i.Ph - P00)) / object->gets0();
                    i.v = Dot(object->getn1(), (i.Ph - P00)) / object->gets1();
                    
                    //wallpaper repeat
                    i.u = i.u - (int)i.u;
                    i.v = i.v - (int)i.v;
                    if (i.u < 0) i.u = 1 + i.u;
                    if (i.v < 0) i.v = 1 + i.v;
                    
                }else{
                    i.th = INFINITY;
                    
                }
                
            }
            break;
        }
            
        case 1:
        {
            //sphere
            float c = Dot((object->getPc() - Pr),(object->getPc() - Pr)) - powf(object->getr(),2);

            float b = Dot(nr, (object->getPc() - Pr));
            float delta = powf(b,2) - c;
            if ( b >= 0 && delta >= 0){
                //intersection occurs
                i.th = b - sqrtf(delta);
                i.Ph = Pr + nr *i.th;
                i.nh = (i.Ph - object->getPc())/object->getr();
                i.intersect = true;
                
                //calculating uv based on ray
                float phi, theta;
                float cosPhi = nr.z;
                if (cosPhi < -1) {
                    cosPhi = -1;
                }
                if (cosPhi > 1) {
                    cosPhi = 1;
                }
                
                phi = pi - acos(cosPhi);
                
                float cosTheta = nr.y / sinf(phi);
                if (cosTheta < -1) {
                    cosTheta = -1;
                }
                if (cosTheta > 1) {
                    cosTheta = 1;
                }
                
                theta = asin(cosTheta) + pi;
                if (theta < 0 ) {
                    theta += pi;
                }
                if (theta > 2*pi) {
                    theta -= 2*pi;
                }
                
                //range from 0 to 1
                i.u = theta / (2 * pi);
                i.v = phi / pi;
            }
            break;
        }
        case 2:
        {
            //triangle
            if (Dot(object->getn2(),nr)!= 0) {
                i.th = ( Dot(object->getn2(),object->getP0()) - Dot(object->getn2(),Pr)) / Dot(object->getn2(),nr);
                if (i.th > 0.0001 ){
                    //intersection occurs
                    i.Ph = Pr + nr * i.th;

                    Vector3D A, A0, A1, A2;
                    A = Cross(object->getP1() - object->getP0(), object->getP2() - object->getP0());
                    A0 = Cross(i.Ph - object->getP1(), i.Ph - object->getP2());
                    A1 = Cross(i.Ph - object->getP2(), i.Ph - object->getP0());
                    A2 = Cross(i.Ph - object->getP0(), i.Ph - object->getP1());
                   
                   float u, v;

                    if(A.x != 0 ){
                        u = A1.x / A.x;
                        v = A2.x / A.x;
                    }else if (A.y != 0){
                        u = A1.y / A.y;
                        v = A2.y / A.y;
                    }else if (A.z != 0){
                        u = A1.z / A.z;
                        v = A2.z / A.z;
                    }else {
                        printf("Something wrong with A calculation...");
                    }
                   
                   
                    if (1-u-v < 1 && 1-u-v > 0 && u < 1 && u > 0 && v < 1 && v > 0) {
                        i.Ph = Pr + nr * i.th;
                        i.nh = (1-u-v) * object->getn2() + u * object->getn1() + v * object->getn2();
                        i.intersect = true;
                        i.u = u;
                        i.v = v;

                    }else{
                        i.Ph = Vector3D(0,0,0);
                        i.th = INFINITY;

                    }
                }else{
                    i.th = INFINITY;
                    
                }
                
            }

            break;
        }
    }
    
    
    return i;
}