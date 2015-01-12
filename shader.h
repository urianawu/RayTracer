#ifndef SHADER_H
#define SHADER_H
#include "Vector3D.h"
#include "ray.h"

class shader
{
public:
    shader();
    void init(Vector3D PL, Point3D CL, Vector3D Ph, Vector3D nh, Vector3D npe);
    float shading(int mode,float a, Vector3D v);
    Point3D MultiplyColor(Point3D a, Point3D b);
    Vector2D findNormals(Vector2D &v1, Vector2D &v2);

    Point3D getColor();

    /*
     * Lambert shading
     * Gooch shading
     */
    void diffuseShading(int mode, float a);

    /*
     * Sharp Circular Specular
     * Rectangular Specular
     */
    void specularShading(int mode, float a);

    void finalColor(Point3D, Point3D, Point3D);

protected:
    Point3D color;
    Point3D thisCL;
    Vector3D thisPL;
    Vector3D thisPh;
    Vector3D thisnh;
    Vector3D thisnpe;
    float c, s;


};

class DSShader: public shader
{
public:
    DSShader();
    //bilinear mixing
    void finalColor(Point3D color0, Point3D color1, Point3D color2);

private:
    Point3D colorD, colorS;
};

/* Phong shading */

class phongShader:public shader
{
public:
    phongShader();
    void finalColor(Point3D theCd, Point3D theCs, Point3D theCa);
private:
    float kd, ks, ka;   //diffuse, specular, ambient coefficient
    Point3D Cd, Cs, Ca; //corresponding color
};




#endif // SHADER_H
