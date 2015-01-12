#include "camera.h"

camera::camera(Vector3D thePe, Vector3D theVview, Vector3D theVup)
{
    Pe = thePe;
    Vview = theVview;
    Vup = theVup;
}

camera::camera()
{
}

void camera::setPe(Vector3D thePe)
{
    Pe = thePe;
}

Vector3D camera::getPe(void)
{
    return Pe;
}

void camera::setVview(Vector3D theVview)
{
    Vview = theVview;
}
void camera::setVup(Vector3D theVup)
{
    Vup = theVup;
}

void camera::setd(float thed)
{
    d = thed;
}

float camera::getd(void)
{
    return d;
}

void camera::setCoordinate(void)
{
    n0 = Cross(Vview,Vup);
    n0.Normalize();
    n1 = Cross(n0,Vview);
    n1.Normalize();
    n2 = Vview;
    n2.Normalize();

}

Vector3D camera::getn0(void)
{
    return n0;
}

Vector3D camera::getn1(void)
{
    return n1;

}

Vector3D camera::getn2(void)
{
    return n2;
}
