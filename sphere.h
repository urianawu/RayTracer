#ifndef SPHERE_H
#define SPHERE_H
#include "geometry.h"

class sphere: public geometry
{
public:
    sphere(Vector3D thePc, float ther);
    Vector3D getPc(void);
    float getr(void);
    int objectType();
private:
    float r;    //radius
    Vector3D Pc;    //center

};

#endif // SPHERE_H
