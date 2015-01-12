#ifndef LIGHT_H
#define LIGHT_H
#include "Vector3D.h"

class light
{
public:
    light();
    void setPL(Vector3D);
    void setColor(Point3D);
    Vector3D getPL();
    Point3D getColor();

private:
    Vector3D PL;
    Point3D color;
};

class pointLight: public light
{
public:
    void setr0(float);
    float getr0();
private:
    float r0;

};
class spotLight: public light
{
public:
    void setnL(Vector3D);
    Vector3D getnL();
private:
    Vector3D nL;
};

#endif // LIGHT_H
