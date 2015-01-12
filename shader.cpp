#include "shader.h"

shader::shader()
{

}

void shader::init(Vector3D PL, Point3D CL, Vector3D Ph, Vector3D nh, Vector3D npe)
{
    thisPL = PL;
    thisCL = CL;
    thisPh = Ph;
    thisnh = nh;
    thisnpe = npe;

}
Point3D shader::getColor()
{
    return color;
}

Point3D shader::MultiplyColor(Point3D a, Point3D b)
{
    Point3D c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    return c;
}

float shader::shading(int mode, float a, Vector3D v)
{
    Vector3D nLh;
    nLh = (thisPL - thisPh).Normalize();
    float cosTheta = Dot(v,nLh);
    float c = 0;
    //switch shading mode
    //0 -  c = cos(theta)
    //1 -  c = (cos(theta) + 1)/2
    switch(mode)
    {
    case 0:
        c = cosTheta;
        if (c < 0) c = 0;
        break;
    case 1:
        c = (cosTheta + 1)/2;
        break;
    default:
        break;
    }

    //control color distribution
    c = powf(c,a);
    return c;
}

void shader::diffuseShading(int mode, float a)
{
    //switch shading mode
    //0 - Lambert shading | c = cos(theta)
    //1 - Gooch shading | c = (cos(theta) + 1)/2
    c = shading(mode, a, thisnh);


}
Vector2D shader::findNormals(Vector2D &v1, Vector2D &v2)
{
    Vector2D v12 = v1-v2;

    v12.Rotate(-90);
    v12.Normalize();
    return v12;
}

void shader::specularShading(int mode, float a)
{
    Vector3D r;
    r = thisnpe - 2 * Dot(thisnh, thisnpe) * thisnh;

    ray rl(thisPh, r);
    //rectangle light area setting
    geometry *p = new plane(thisPL,Vector3D(0,0,-1));
    float Sx = 20;
    float Sy = 20;
    Vector3D n0(1,0,0);
    Vector3D n1(0,1,0);
    //shoot a ray to rectangle plane
    rl.shoot(p,1);

    //switch shading mode
    //0 - Sharp Circular Specular
    //1 - Rectangular Specular
    //2 - French window Specular
    //3 - star
    switch(mode)
    {
    case 0:
    {
        s = shading(0, a, r);
        if (s < 0.5) s = 0;
        break;
    }
    case 1:
    {
        if (rl.i.hasIntersect) {
            Vector3D P00 = thisPL - Sx/2*n0 - Sy/2*n1;
            float x = Dot((rl.i.Ph - P00),n0) /Sx;
            float y = Dot((rl.i.Ph - P00),n1) /Sy;
            if(x>=0 && x<=1 && y>=0 && y<=1)
                s = 1;
            else
                s = 0;
        }
        break;
    }
    case 2:
    {

        if (rl.i.hasIntersect) {
            float x = 2/Sx * Dot((rl.i.Ph - thisPL), n0);
            float y = 2/Sy * Dot((rl.i.Ph - thisPL), n1);
            if(fabs(x)>= 0.2 && fabs(x) <=1 && fabs(y)>= 0.2 && fabs(y) <=1)
                s = 1;
            else
                s = 0;
        }
        break;
    }
    case 3:
    {
        Vector2D p0(300.0/640, 370.0/480);
        Vector2D p1(190.0/640, 260.0/480);
        Vector2D p2(230.0/640, 170.0/480);
        Vector2D p3(404.0/640, 180.0/480);
        Vector2D p4(390.0/640, 330.0/480);

        Vector2D p02 = findNormals(p2,p0);
           Vector2D p13 = findNormals(p3,p1);
           Vector2D p24 = findNormals(p4,p2);
           Vector2D p30 = findNormals(p0,p3);
           Vector2D p41 = findNormals(p1,p4);

        if (rl.i.hasIntersect) {
            float x = 2/Sx * Dot((rl.i.Ph - thisPL), n0);
            float y = 2/Sy * Dot((rl.i.Ph - thisPL), n1);
            int flag = 0;
            Vector2D p(x,y);
            if (Dot(p02,(p-p0)) <= 0)
                flag +=1;
            if (Dot(p13,(p-p1)) <= 0)
                flag +=1;
            if (Dot(p24,(p-p2)) <= 0)
                flag +=1;
            if (Dot(p30,(p-p3)) <= 0)
                flag +=1;
            if (Dot(p41,(p-p4)) <= 0)
                flag +=1;

            if(flag >= 4)
                s = 1;
            else
                s = 0;
        }
    }
    default:
        break;
    }


}
void shader::finalColor(Point3D, Point3D, Point3D)
{

}

DSShader::DSShader()
{

}

void DSShader::finalColor(Point3D color0, Point3D color1, Point3D color2)
{
    Vector3D Cdl;
    //calculate object diffuse color
    colorD = (1 - c) * color0 + c * color1;
    colorS = color2;

    Cdl = MultiplyColor(colorD, thisCL);

//    Cdl.x = 0.9*colorD.x + 0.1*thisCL.x;
//    Cdl.y = 0.9*colorD.y + 0.1*thisCL.y;
//    Cdl.z = 0.9*colorD.z + 0.1*thisCL.z;

    color = Cdl * (1 - s) + colorS * s;
}
phongShader::phongShader()
{

}

void phongShader::finalColor(Point3D theCd, Point3D theCs, Point3D theCa)
{
    kd = 0.5;
    ks = 0.3;
    ka = 0.2;
    Cd = theCd;
    Cs = theCs;
    Ca = theCa;

    color = (kd * c )* MultiplyColor(Cd, thisCL) + (ks * s) * MultiplyColor(Cs, thisCL) + ka * Ca;
}
