#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "Vector3D.h"
#include "shader.h"

class geometry
{
public:
    geometry();
    virtual Vector3D getPc(){return Vector3D(0,0,0);}
    virtual float getr(){return 0;}
    
    virtual Vector3D getP0(void){return Vector3D(0,0,0);}
    virtual Vector3D getP1(void){return Vector3D(0,0,0);}
    virtual Vector3D getP2(void){return Vector3D(0,0,0);}
    
    virtual Vector3D getn0(void){return Vector3D(0,0,0);}
    virtual Vector3D getn1(void){return Vector3D(0,0,0);}
    virtual Vector3D getn2(void){return Vector3D(0,0,0);}
    
    virtual Vector3D getnAt0(void){return Vector3D(0,0,0);}
    virtual Vector3D getnAt1(void){return Vector3D(0,0,0);}
    virtual Vector3D getnAt2(void){return Vector3D(0,0,0);}
    
    virtual float gets0(){return 0;};
    virtual float gets1(){return 0;};
    virtual int objectType(){return NULL;}
    
    void setShader(shader *shader);
    shader* getShader();
private:
    shader *s;
};

#endif // GEOMETRY_H
