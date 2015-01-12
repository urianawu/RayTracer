#include "quadric.h"

quadric::quadric()
{
}

void quadric::setShape(int shape)
{
    switch(shape)
    {
    case 0:
        //plane
        a02 = 0; a12 = 0; a22 = 0; a21 = 1; a00 = 0;
        break;
    case 1:
        //Generalized Cylinder
        a02 = 1; a12 = 1; a22 = 0; a21 = 0; a00 = -1;
        break;
    case 2:
        //Ellipsoid
        a02 = 1; a12 = 1; a22 = 1; a21 = 0; a00 = -1;
        break;
    case 3:
        //Cone
        a02 = 1; a12 = 1; a22 = -1; a21 = 0; a00 = 0;
        break;
    case 4:
        //Hyperboloid of one sheet
        a02 = 1; a12 = 1; a22 = -1; a21 = 0; a00 = -1;
        break;
    case 5:
        //Hyperboloid of two sheet
        a02 = 1; a12 = 1; a22 = -1; a21 = 0; a00 = 1;
        break;
    case 6:
        //Elliptic Paraboloid
        a02 = 1; a12 = 1; a22 = 0; a21 = -1; a00 = 0;
        break;
    case 7:
        //Hyperbolic Paraboloid
        a02 = 1; a12 = -1; a22 = 0; a21 = -1; a00 = 0;
        break;
    }

}

void quadric::setCenter(Vector3D Pc)
{
    this->Pc = Pc;
}

void quadric::setScale(float s0, float s1, float s2)
{
    this->s0 = s0;
    this->s1 = s1;
    this->s2 = s2;
}

void quadric::setRotation(Vector3D n0, Vector3D n1, Vector3D n2)
{
    this->n0 = n0;
    this->n1 = n1;
    this->n2 = n2;
}

float quadric::F(Vector3D P)
{
    float result;

    result = (float)a02 * powf((Dot(n0,(P-Pc))/s0),2)
            + (float)a12 * powf((Dot(n1,(P-Pc))/s1),2)
            + (float)a22 * powf((Dot(n2,(P-Pc))/s2),2)
            + (float)a21 * (Dot(n2,(P-Pc))/s2)
            + (float)a00;
    return result;
}

void quadric::tracing(ray r)
{
    i.hasIntersect = false;
    i.nh = Vector3D();
    i.Ph = Vector3D();
    i.th = 0;

    float A = (float)a02 * powf((Dot(n0,r.getnr())/s0),2)
            + (float)a12 * powf((Dot(n1,r.getnr())/s1),2)
            + (float)a22 * powf((Dot(n2,r.getnr())/s2),2);

    float B = (float)a02 * 2 * Dot(n0,r.getnr()) * (Dot(n0,(r.getPr()-Pc)))/powf(s0,2)
            + (float)a12 * 2 * Dot(n1,r.getnr()) * (Dot(n1,(r.getPr()-Pc)))/powf(s1,2)
            + (float)a22 * 2 * Dot(n2,r.getnr()) * (Dot(n2,(r.getPr()-Pc)))/powf(s2,2)
            + (float)a21 * Dot(n2,r.getnr())/s2;

    float C = (float)a02 * powf((Dot(n0,(r.getPr()-Pc))/s0),2)
            + (float)a12 * powf((Dot(n1,(r.getPr()-Pc))/s1),2)
            + (float)a22 * powf((Dot(n2,(r.getPr()-Pc))/s2),2)
            + (float)a21 * (Dot(n2,(r.getPr()-Pc))/s2)
            + (float)a00;
    float delta = powf(B,2) - 4 * A * C;

    if ( delta >= 0)
    {
        float t1;
        float t2;

        if(A == 0){
            //t1 = t2 = (Dot(n2,Pc) - Dot(n2,r.getPr())) / Dot(n2,r.getnr());
        }else {
        t1 = (-B - sqrtf(powf(B,2) - 4 * A * C))/(2 * A);
        t2 = (-B + sqrtf(powf(B,2) - 4 * A * C))/(2 * A);
        }
        if( t1 > 0 && t2 > 0){
            //intersection occurs
            float th = fminf(t1, t2);
            i.th = th;
            i.Ph = r.getPr() + r.getnr() * th;
            i.nh = (float)a02 * 2 * (Dot(n0,(i.Ph-Pc)))/powf(s0,2) * n0
                    + (float)a12 * 2 * (Dot(n1,(i.Ph-Pc)))/powf(s1,2) * n1
                    + (float)a22 * 2 * (Dot(n2,(i.Ph-Pc)))/powf(s2,2) * n2
                    + (float)a21 / s2 * n2;
            i.hasIntersect = true;
        }
    }

}

