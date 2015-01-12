#ifndef LIGHT_H
#define LIGHT_H
#include "Vector3D.h"

class light
{
public:
    light();
    virtual void setPL(Vector3D){}
    virtual Vector3D getPL(){return Vector3D(0,0,0);}

    virtual void setnL(Vector3D){}
    virtual Vector3D getnL(){return Vector3D(0,0,0);}
    
    virtual void setTheta(float){}
    virtual float getTheta(){return 0.0;}
    
    virtual void setPlane(Vector3D n0, Vector3D n1, float S0, float S1){}
    virtual void setSampleSize(int){}
    virtual int getSampleSize(){return 0;}
    virtual Vector3D getP00(){return Vector3D(0,0,0);}
    virtual Vector3D getn0(){return Vector3D(0,0,0);}
    virtual Vector3D getn1(){return Vector3D(0,0,0);}
    virtual float getS0(){return 0;}
    virtual float getS1(){return 0;}
    
    void setColor(Point3D);
    Point3D getColor();
    virtual int lightType(){return NULL;}

private:
    Point3D color;
};

class pointLight: public light
{
public:
    using light::light;
    void setPL(Vector3D);
    Vector3D getPL();
    
    int lightType(){return 0;}
private:
    Vector3D PL;

};
class directionalLight: public light
{
public:
    using light::light;

    void setnL(Vector3D);
    Vector3D getnL();
    int lightType(){return 1;}
private:
    Vector3D nL;
};

class spotLight: public light
{
public:
    using light::light;
    
    void setPL(Vector3D);
    Vector3D getPL();
    void setnL(Vector3D);
    Vector3D getnL();

    void setTheta(float);
    float getTheta();
    int lightType(){return 2;}
private:
    Vector3D PL;
    Vector3D nL;

    float theta;
};

class areaLight: public light
{
public:
    using light::light;
    void setPL(Vector3D);
    void setSampleSize(int);
    void setPlane(Vector3D n0, Vector3D n1, float S0, float S1);
    
    Vector3D getPL();
    Vector3D getP00();
    Vector3D getn0();
    Vector3D getn1();
    float getS0();
    float getS1();
    int getSampleSize();


    int lightType(){return 3;}
private:
    Vector3D PL;
    Vector3D P00;
    Vector3D n0, n1;
    float S0, S1;
    int numOfSample;

};
#endif // LIGHT_H
