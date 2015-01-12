#ifndef PLANE_H
#define PLANE_H
#include "geometry.h"

class plane : public geometry
{
public:
    plane(Vector3D thePc, Vector3D then0, Vector3D then1, Vector3D then2, float s0, float s1);
    
    Vector3D getPc(void);
    
    Vector3D getn0(void);
    Vector3D getn1(void);
    Vector3D getn2(void);
    
    float gets0();
    float gets1();

    int objectType();
private:
    Vector3D n0, n1, n2;
    float s0, s1;
    Vector3D Pc;
};

#endif // PLANE_H
