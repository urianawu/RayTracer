#include "plane.h"

plane::plane(Vector3D thePc, Vector3D then0, Vector3D then1, Vector3D then2, float s0, float s1)
{
    Pc = thePc;
    n0 = then0;
    n1 = then1;
    n2 = then2;
    this->s0 = s0;
    this->s1 = s1;
}

Vector3D plane::getPc(void){
    return Pc;
}

Vector3D plane::getn0(void){
    return n0;
}
Vector3D plane::getn1(void){
    return n1;
}
Vector3D plane::getn2(void){
    return n2;
}

float plane::gets0(){
    return s0;
}
float plane::gets1(){
    return s1;
}
int plane::objectType(){
    return 0;
}