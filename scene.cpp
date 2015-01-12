#include "scene.h"

scene::scene()
{
    w = 100;
    h = 100;

}
//Compute the center of view-port pc= pe+ d*n2
Vector3D scene::getPc(void)
{
    return Pc;

}

//Compute the lower left corner of the view-port: p0,0.
Vector3D scene::getP00(void)
{
    return P00;
}

void scene::setPc(Vector3D thePc)
{
    Pc = thePc;
}

void scene::setP00(Vector3D theP00)
{
    P00 = theP00;
}

float scene::getSx(void)
{
    return Sx;
}

float scene::getSy(void)
{
    return Sy;
}

unsigned char * scene::finalImage(void)
{
    image = new unsigned char[w * h * 3];
    for (int i = 0; i< w * h * 3; i++){
        image[i] = 100;
    }
    return image;
}

void scene::setImageSize(int thew, int theh)
{
    w = thew;
    h = theh;
}
void scene::setWindowWidth(float theSx)
{
    Sx = theSx;
    Sy = theSx *((float)h/(float)w);
}
