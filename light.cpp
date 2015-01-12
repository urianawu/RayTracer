#include "light.h"

light::light()
{
    
}


void light::setColor(Point3D theColor)
{
    color = theColor;
}

Point3D light::getColor()
{
    return color;
}

void pointLight::setPL(Vector3D thePL)
{
    PL = thePL;
}
Vector3D pointLight::getPL()
{
    return PL;
}


void directionalLight::setnL(Vector3D thenL)
{
    nL = thenL;
}

Vector3D directionalLight::getnL()
{
    return nL;
}

void spotLight::setPL(Vector3D thePL)
{
    PL = thePL;
}
Vector3D spotLight::getPL()
{
    return PL;
}

void spotLight::setnL(Vector3D thenL)
{
    nL = thenL;
}

Vector3D spotLight::getnL()
{
    return nL;
}

void spotLight::setTheta(float theta){
    this->theta = theta;
}

float spotLight::getTheta(){
    return theta;
}


void areaLight::setPL(Vector3D thePL)
{
    PL = thePL;
}

void areaLight::setSampleSize(int sample)
{
    numOfSample = sample;
}

Vector3D areaLight::getPL()
{
    return PL;
}

void areaLight::setPlane(Vector3D n0, Vector3D n1, float S0, float S1)
{
    this->n0 = n0;
    this->n1 = n1;
    this->S0 = S0;
    this->S1 = S1;
    P00 = PL - S0/2 * n0 - S1/2 * n1;
}

Vector3D areaLight::getP00()
{
    return P00;
}

Vector3D areaLight::getn0()
{
    return n0;
}

Vector3D areaLight::getn1()
{
    return n1;
}
float areaLight::getS0()
{
    return S0;
}
float areaLight::getS1()
{
    return S1;
}

int areaLight::getSampleSize()
{
    return numOfSample;
}
