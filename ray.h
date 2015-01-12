#ifndef RAY_H
#define RAY_H
#include "plane.h"
#include "sphere.h"

class ray
{
public:
    ray(Vector3D, Vector3D);
    Vector3D getPr();
    Vector3D getnr();
    void shoot(geometry *g, int type);

    struct intersect{
        Vector3D Ph;
        Vector3D nh;
        float th;
        bool hasIntersect;
    }i;

private:
    Vector3D Pr;
    Vector3D nr;

};

#endif // RAY_H
