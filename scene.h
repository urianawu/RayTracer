#ifndef SCENE_H
#define SCENE_H

#include"Vector3D.h"

class scene
{
public:
    scene();
    Vector3D getPc(void);
    Vector3D getP00(void);
    void setPc(Vector3D thePc);
    void setP00(Vector3D  theP00);
    float getSx(void);
    float getSy(void);
    unsigned char * finalImage(void);
    void setImageSize(int thew, int theh);
    void setWindowWidth(float theSx);

private:
    unsigned char * image;
    int w,h;    //Xmax, Ymax
    float Sx, Sy;
    Vector3D P00;
    Vector3D Pc;

};

#endif // SCENE_H
