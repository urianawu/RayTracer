#include "mainwindow.h"
#include "glwidget.h"
#include <QApplication>
#include <GLUT/glut.h>

#include "pixmap.h"

#include "scene.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "ray.h"

#include "geometry.h"
#include "sphere.h"
#include "plane.h"

#include "quadric.h"

camera thiscam;
scene thisScene;
pointLight thisLight;

int Num_Samples_x = 4;
int Num_Samples_y = 4;



struct object {
    geometry *g;
    shader *s;
    int id;
    float th;
}s1, p1;

struct objectQ {
    quadric q;
    shader *s;
    int id;
    float th;
}o1, o2;

struct intersection {
    float minth;
    int minid;
    bool intersect;
};

void setCamera()
{
    thiscam.setPe(Vector3D(10,0,0));
    thiscam.setVup(Vector3D(0,0,1));
    thiscam.setVview(Vector3D(-1,0,0));
    thiscam.setd(4);
    thiscam.setCoordinate();
}
void setScene()
{
    thisScene.setImageSize(W,H);
    thisScene.setWindowWidth(10);   //Sx, Sy
    thisScene.setPc( thiscam.getPe() + thiscam.getd()*thiscam.getn2());
    thisScene.setP00( thisScene.getPc() - thiscam.getn0()*(thisScene.getSx()/2) - thiscam.getn1()*(thisScene.getSy()/2));

}

void setLight()
{
    thisLight.setPL(Vector3D(20,6,18));
    thisLight.setr0(1.0);
    thisLight.setColor(Point3D(1, 1, 1));
}

shader* setShader(int type, Vector3D thenpe, Vector3D thePh, Vector3D thenh)
{
    shader * thes;

    switch(type){
    case 0:
    {
        DSShader b1;
        b1.init(thisLight.getPL(),thisLight.getColor(),thePh,thenh,thenpe);
        b1.diffuseShading(1,1);
        b1.specularShading(2,20);
        b1.finalColor(Point3D(0,0,1),Point3D(1,1,0),Point3D(1,1,1));
        thes = &b1;
        break;
    }
    case 1:
    {
        DSShader b2;
        b2.init(thisLight.getPL(),thisLight.getColor(),thePh,thenh,thenpe);
        b2.diffuseShading(0,1);
        b2.specularShading(3,5);
        b2.finalColor(Point3D(0.1,0.1,0.2),Point3D(1,1,0.9), Point3D(1,1,1));

        thes = &b2;
        break;
    }
    case 2:
    {
        phongShader phong;
        phong.init(thisLight.getPL(),thisLight.getColor(),thePh,thenh,thenpe);
        phong.diffuseShading(0,1);
        phong.specularShading(0,5);
        phong.finalColor(Point3D(1,0,0),Point3D(1,1,1), Point3D(0.5,0.5,0));

        thes = &phong;
        break;

    }
    case 3:
    {
        phongShader phong;
        phong.init(thisLight.getPL(),thisLight.getColor(),thePh,thenh,thenpe);
        phong.diffuseShading(0,1);
        phong.specularShading(2,5);
        phong.finalColor(Point3D(1,0.3,0.3),Point3D(1,1,1), Point3D(0.5,0.5,0));

        thes = &phong;
        break;
    }
    default:
    {
        phongShader bilinear;
        thes = &bilinear;
        break;
    }
    }

    return thes;
}

void setObjects()
{

    s1.g = new sphere(Vector3D(0, 0, 0),3);
    s1.id = 0;

    p1.g = new plane(Vector3D(0,0,-4),Vector3D(0,0,1));
    p1.id = 1;
}
void setQuadrics()
{
    o1.q.setCenter(Vector3D(0,0,0));
    o1.q.setShape(3);
    o1.q.setScale(2,5,4);
    o1.q.setRotation(Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1));
    o1.id = 0;
}
void checkInsideObject()
{
    if (o1.q.F(thiscam.getPe()) < 0) {
        printf("\nEye point is inside the object\n");
        exit(0);
    }
    if (o1.q.F(thisLight.getPL()) < 0) {
        printf("\nLight is inside the object\n");
        exit(0);
    }
}

void raytrace(Vector3D thePe, Vector3D thenpe, int objectType, object &obj, intersection &in)
{
    ray thisRay(thePe, thenpe);
    bool intersect = false;
    int shaderType;
    switch(obj.id)
    {
    case 0:
        shaderType = 0;
        break;
    case 1:
        shaderType = 1;
        break;
    case 2:
        shaderType = 2;
        break;
    default:
        shaderType = 0;
        break;
    }

    //object type
    //0 - sphere    1 - plane   2 - quadric
    thisRay.shoot(obj.g,objectType);
    if (thisRay.i.hasIntersect) {
        obj.th = thisRay.i.th;
        obj.s = setShader(shaderType,thenpe,thisRay.i.Ph,thisRay.i.nh);
        intersect = true;
    }

//    switch(objectType)
//    {
//    case 0:
//    {
//        float c = Dot((obj.g->getPc() - thePe),(obj.g->getPc() - thePe)) - powf(obj.g->getr(),2);
//        if (c < 0) {
//            printf("\nEye point is inside the sphere\n");
//            exit(0);
//        }
//        float b = Dot(thenpe, (obj.g->getPc() - thePe));
//        float delta = powf(b,2) - c;
//        if ( b >= 0 && delta >= 0){
//            //intersection occurs
//            float th = b - sqrtf(delta);
//            obj.th = th;
//            Ph = thePe + thenpe * th;
//            nh = (Ph - obj.g->getPc())/obj.g->getr();
//            obj.s = setShader(0,thenpe,Ph,nh);
//            intersect = true;
//        }
//        break;
//    }
//    case 1:
//    {
//        if (thisRay.i.hasIntersect) {
//            obj.th = thisRay.i.th;
//            Ph = thisRay.i.Ph;
//            nh = thisRay.i.nh;
//            obj.s = setShader(obj.id,thenpe,Ph,nh);
//            intersect = true;
//        }
//        float th = (Dot(obj.g->getn0(),obj.g->getP0()) - Dot(obj.g->getn0(),thePe)) / Dot(obj.g->getn0(),thenpe);
//        if (th > 0){
//            //intersection occurs
//            obj.th = th;
//            Ph = thePe + thenpe *th;
//            nh = obj.g->getn0();
//            obj.s = setShader(1,thenpe,Ph,nh);
//            intersect = true;
//        }
//        break;
//    }
//    case 2:
//        break;
//    default:
//        break;
//    }

    if(intersect)
    {
        if(in.minid == -1) {
            in.minth = obj.th;
            in.minid = obj.id;
        }
        else {
            if(in.minth > obj.th) {
                in.minth = obj.th;
                in.minid = obj.id;
            }
        }

    }
}

void raytrace(ray thisRay, objectQ &obj, intersection &in)
{
    int shaderType;
    switch(obj.id)
    {
    case 0:
        shaderType = 3;
        break;
    case 1:
        shaderType = 1;
        break;
    case 2:
        shaderType = 2;
        break;
    default:
        shaderType = 0;
        break;
    }

    obj.q.tracing(thisRay);
    if (obj.q.i.hasIntersect) {
        obj.th = obj.q.i.th;
        obj.s = setShader(shaderType,thisRay.getnr(),obj.q.i.Ph,obj.q.i.nh);
        in.intersect = true;
    }
    if(in.intersect)
    {
        if(in.minid == -1) {
            in.minth = obj.th;
            in.minid = obj.id;
        }
        else {
            if(in.minth > obj.th) {
                in.minth = obj.th;
                in.minid = obj.id;
            }
        }

    }
}

void setPixels(int width, int height)
{
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float color[] = {0.0,0.0,0.0};
            float rx = rand()/RAND_MAX;
            float ry = rand()/RAND_MAX;
            for(float i = 0; i < Num_Samples_x; i++){
                for(float j = 0; j < Num_Samples_y; j++){
                    float x_sample=(rx+i)/Num_Samples_x;
                    float y_sample=(ry+j)/Num_Samples_y;
                    Vector2D p(x+x_sample,y+y_sample);
                    float x_normalized = p.x/width;
                    float y_normalized = p.y/height;

                    //set Pp, npe
                    Vector3D Pp = thisScene.getP00()
                            + (float)thisScene.getSx() * x_normalized * thiscam.getn0()
                            + (float)thisScene.getSy() * y_normalized * thiscam.getn1();
                    Vector3D npe = (Pp - thiscam.getPe()).Normalize();

                    intersection in;
                    ray r(thiscam.getPe(),npe);
                    in.minid = -1;
                    in.minth = 0.0;
                    in.intersect = false;

                    //printf("Pp %f,%f,%f Npe %f,%f,%f\n",Pp.x,Pp.y,Pp.z,npe.x,npe.y,npe.z);
                    //raytrace(thiscam.getPe(), npe, 0, s1, in);
                    //raytrace(thiscam.getPe(), npe, 1, p1, in);

                    raytrace(r, o1, in);

                    Point3D cF;
//                    if(in.minid == 0) cF = s1.s->getColor();
//                    else if(in.minid == 1) cF = p1.s->getColor();
                    if(in.minid == 0) cF = o1.s->getColor();

                    Point3D cB(0.2, 0.2, 0.2);
                    //printf("CF %f %f %f\n",cF.x, cF.y, cF.z);
                    if (in.intersect){
                        color[0] += cF.x;
                        color[1] += cF.y;
                        color[2] += cF.z;
                    }else{
                        color[0] += cB.x;
                        color[1] += cB.y;
                        color[2] += cB.z;
                    }

                }
            }
            pixels[i++] = 255 * color[0] / (Num_Samples_x*Num_Samples_y);
            pixels[i++] = 255 * color[1] / (Num_Samples_x*Num_Samples_y);
            pixels[i] = 255 * color[2] / (Num_Samples_x*Num_Samples_y);
        }
    }
}

int main(int argc, char *argv[])
{
    //logic
    setCamera();
    setScene();
    //setObjects();
    setQuadrics();
    setLight();
    checkInsideObject();
    setPixels(W,H);

    //UI
    QApplication a(argc, argv);
    //glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);


    MainWindow w;
    w.show();


    return a.exec();
}
