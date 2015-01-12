#ifndef PLANE_H
#define PLANE_H
#include "geometry.h"

class plane : public geometry
{
public:
    plane(Vector3D theP0, Vector3D then0);
    Vector3D getP0(void);
    Vector3D getn0(void);
    Vector3D getPc(void){return Vector3D(0,0,0);}
    float getr(void){return 0.0;}
private:
    Vector3D n0;
    Vector3D P0;
};

#endif // PLANE_H
