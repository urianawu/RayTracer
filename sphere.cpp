#include "sphere.h"

sphere::sphere(Vector3D thePc, float ther)
{
    Pc = thePc;
    r = ther;
}
Vector3D sphere::getPc(void)
{
    return Pc;
}
float sphere::getr(void)
{
    return r;
}
int sphere::objectType(){
    return 1;
}