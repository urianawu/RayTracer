#ifndef QUADRIC_H
#define QUADRIC_H
#include "ray.h"


class quadric
{
public:
    quadric();
    void setShape(int shape);
    void setCenter(Vector3D Pc);
    void setScale(float s0, float s1, float s2);
    void setRotation(Vector3D n0, Vector3D n1, Vector3D n2);
    float F(Vector3D p);
    void tracing(ray r);

    struct intersect{
        Vector3D Ph;
        Vector3D nh;
        float th;
        bool hasIntersect;
    }i;

private:
    int a02, a12, a22, a21, a00;
    float s0, s1, s2;
    Vector3D Pc;

    //local coordinates
    Vector3D n0, n1, n2;
};

#endif // QUADRIC_H
