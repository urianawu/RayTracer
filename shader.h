#ifndef SHADER_H
#define SHADER_H
#include "Vector3D.h"
#include "intersection.h"
#include "light.h"
#include "texture.h"
#include "perlin.h"
#include "colorSpaceConversion.h"
#include <vector>
#include <math.h>       /* acos */
#define pi 3.14159265358

class ray;
class geometry;

class shader
{
public:
    shader(float, Point3D, Point3D, Point3D, int dMode, float da, int sMode, float sa, char* , float kr, bool glossy,float kt, float eta, bool envir, bool bump, float ki);
    float getTransparency();
    
    virtual Point3D getColor(light *l, intersection i, ray thisray, std::vector<geometry*>) = 0;
    virtual Point3D getCd(intersection){return Point3D(0,0,0);}
    virtual Point3D getCs(){return Point3D(0,0,0);}
    void init(light *l, intersection i, Vector3D nr);
protected:
    float doShadowCasting( std::vector<geometry *> objects, int objectId);
    float calCosTheta(int mode,float a, Vector3D v, Vector3D n);
    Point3D MultiplyColor(Point3D a, Point3D b);
    Vector2D findNormals(Vector2D &v1, Vector2D &v2);
    void findCoordinateAt(geometry* sphere, intersection i, float phiX, float  thetaX, float phiY, float thetaY, Vector3D &Nx, Vector3D &Ny);
    void handleColorOverflow(Point3D &color);

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
    Point3D doReflection(ray thisray, Vector3D r, std::vector<geometry*> objects, int objectId, light* l, float u, float v);
    Point3D doReflection(ray thisray,std::vector<geometry*> objects, int objectId, light* l, float u, float v);

    Point3D doRefraction(ray thisray, std::vector<geometry*> objects, intersection i, light* l, float u, float v);
    
    void calUV(Vector3D thisraynh, float &u, float &v);
    void doBumpMapping(geometry* object, intersection &i);
    Point3D doIridescence();

    int dMode;
    float da;
    int sMode;
    float sa;
    float kr;
    float kt;
    float eta;
    float ki;

    Point3D color;
    float alpha;
    
    Point3D thisCL;
    Vector3D thisPL;
    Vector3D thisNL;

    Vector3D thisPh;
    Vector3D thisnh;
    Vector3D thisnpe;
    Vector3D r;
    
    float c, s;
    Point3D c0;
    Point3D c1;
    Point3D c2;

    texture tex;
    texture environmentalMap;
    texture bumpMap;
    perlin pNoise;

    bool environmentalMapping;
    bool bumpMapping;
    bool glossy;

};

class DSShader: public shader
{
public:
    using shader::shader;
    Point3D getColor(light *l, intersection i, ray thisray, std::vector<geometry*>);


private:
    Point3D getCd(intersection);
    Point3D getCs();
    Point3D colorD, colorS;

};

/* Phong shading */

class phongShader: public shader
{
public:
    using shader::shader;
    Point3D getColor(light *l, intersection i, ray thisray, std::vector<geometry*>);

private:
    float kd, ks, ka;   //diffuse, specular, ambient coefficient
};




#endif // SHADER_H
