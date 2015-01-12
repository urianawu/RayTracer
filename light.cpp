#include "light.h"

light::light()
{
}

void light::setPL(Vector3D thePL)
{
    PL = thePL;
}

void light::setColor(Point3D theColor)
{
    color = theColor;
}

Vector3D light::getPL()
{
    return PL;
}
Point3D light::getColor()
{
    return color;
}

void pointLight::setr0(float ther0)
{
    r0 = ther0;
}
float pointLight::getr0()
{
    return r0;
}

void spotLight::setnL(Vector3D thenL)
{
    nL = thenL;
}

Vector3D spotLight::getnL()
{
    return nL;
}
