#include "plane.h"

plane::plane(Vector3D theP0, Vector3D then0)
{
    P0 = theP0;
    n0 = then0;
}

Vector3D plane::getP0(void){
    return P0;
}

Vector3D plane::getn0(void){
    return n0;
}
