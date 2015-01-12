#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3D.h"
class camera
{
public:
    camera(Vector3D thePe, Vector3D theVview, Vector3D theVup);
    camera();
    void setPe(Vector3D thePe);
    Vector3D getPe(void);
    void setVview(Vector3D theVview);
    void setVup(Vector3D theVup);
    void setd(float thed);
    float getd(void);
    void setCoordinate(void);
    Vector3D getn0(void);
    Vector3D getn1(void);
    Vector3D getn2(void);

private:
    Vector3D Pe;
    Vector3D Vview;
    Vector3D Vup;
    float d;    //distance
    Vector3D n0;
    Vector3D n1;
    Vector3D n2;

};

#endif // CAMERA_H
