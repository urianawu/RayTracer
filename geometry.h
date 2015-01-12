#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "Vector3D.h"

class geometry
{
public:
    geometry();
    virtual Vector3D getPc() = 0;
    virtual float getr() = 0;
    virtual Vector3D getP0(void) = 0;
    virtual Vector3D getn0(void) = 0;
};

#endif // GEOMETRY_H
