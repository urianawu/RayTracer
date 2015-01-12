#include <iostream>
#include <thread>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "ray.h"
#include "intersection.h"

#include "geometry.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "objLoader.h"

#define W 1280
#define H 720
#define PI  3.14159265358

static const int num_threads = 2;

int * MBpixels = new int[ W * H * 3];
unsigned char * pixels = new unsigned char[ W * H * 3];
unsigned char * FinalPixels = new unsigned char[ W * H * 3];

using namespace std;

camera cam;
scene scn;
ppmview mask;   //camera painting image


std::vector<light *> lights;
std::vector<geometry *> objects;

int Num_Samples_x = 4;
int Num_Samples_y = 4;


float s1z, theta2, theta3, r2, r3;


float iorAnim;
void setCamera(Vector3D Pe)
{
    cam.setPe(Pe);
    cam.setVup(Vector3D(0,0,1));
    cam.setVview((Point3D(0,0,0) - Pe).Normalize());

    cam.setd(10);
    cam.setCoordinate();
    
    
    //camera painting
    mask.readPPM("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/mud.ppm");
}

void setScene(float heightOffset, float PcOffset)
{
    scn.setImageSize(W,H *heightOffset);
    scn.setWindowWidth(10);   //Sx, Sy
    scn.setPc( cam.getPe() + cam.getd()*cam.getn2() - PcOffset * 1.0/num_threads *cam.getn1() *scn.getSy());
    scn.setP00( scn.getPc() - cam.getn0()*(scn.getSx()/2) - cam.getn1()*(scn.getSy()/2));
}

void setLight()
{
    light * pLight1, *pLight2, *pLight3, *dLight, *sLight, *aLight;
    pLight1 = new pointLight();
    pLight1->setPL(Vector3D(20,10,18));
    pLight1->setColor(Point3D(1, 1, 1));
    lights.push_back(pLight1);
    
    pLight2 = new pointLight();
    pLight2->setPL(Vector3D(20,-10,18));
    pLight2->setColor(Point3D(1, 1, 1));
    //lights.push_back(pLight2);
   
    pLight3 = new pointLight();
    pLight3->setPL(Vector3D(20,0,0));
    pLight3->setColor(Point3D(1, 1, 1));
    //lights.push_back(pLight3);
    
    sLight = new spotLight();
    sLight->setPL(Vector3D(20,0,18));
    sLight->setnL(Vector3D(-1, 0, -1));
    sLight->setTheta(PI/10);
    sLight->setColor(Point3D(1, 1, 1));
    //lights.push_back(sLight);
    
    dLight = new directionalLight();
    dLight->setnL(Vector3D(-1, 0, 0));
    dLight->setColor(Point3D(1, 1, 1));
    //lights.push_back(dLight);
    
    aLight = new areaLight();
    aLight->setPL(Vector3D(20,0,18));
    aLight->setPlane(Vector3D(0,1,0), Vector3D(-1,0,0), 5, 5);
    aLight->setSampleSize(2);
    aLight->setColor(Point3D(1,1,1));
    //lights.push_back(aLight);
}

void setObjects(int mi)
{
    geometry *p1, *p2, *p3, *p4, *p5, *s1, *s2, *s3, *s4;
    if (mi > 24) {
        s1z += 0.1;
    }else {
        s1z -= 0.1;
    }
    float s2x, s2y, s3x, s3y;
    theta2 += (float) pi / 15.0;
    theta3 += (float) pi / 10.0;
    if (theta2 > 2 * pi) {
        theta2 -= 2*pi;
    }
    if (theta3 > 2 * pi) {
        theta3 -= 2*pi;
    }
    s2x = r2 * cosf(theta2);
    s2y = r2 * sinf(theta2);
    s3x = r3 * cosf(theta3);
    s3y = r3 * sinf(theta3);

    p1 = new plane(Vector3D(0,0,-20),Vector3D(0,1,0), Vector3D(-1,0,0), Vector3D(0,0,1), 107,60);
    p1->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(0.2,0.2,0.2), 1, 2, 1, 5.0, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/bricks.ppm",0,false,0,1.33, false, false, 0));
    objects.push_back(p1);
    
    p2 = new plane(Vector3D(-30,0,0),Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(1,0,0),120, 100);
    p2->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 1, 20.0, 1, 5.0, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/newyork.ppm",1,false,0,1, false, false, 0));
    objects.push_back(p2);
    
     p3 = new plane(Vector3D(0,-20,0),Vector3D(0,0,1),Vector3D(1,0,0),Vector3D(0,1,0),107,60);
     p3->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(0.2,0.2,0.2), 1, 2, 1, 5.0, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/bricks.ppm", 0,false,0,1, false, false, 0));
     //objects.push_back(p3);
     
     p4 = new plane(Vector3D(0,20,0),Vector3D(0,0,1), Vector3D(-1,0,0),Vector3D(0,-1,0),107,60);
     p4->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 0, 10.0, 1, 5.0, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/bricks.ppm",0,false,0, 1.33, false, false, 0));
     //objects.push_back(p4);
    
    p5 = new plane(Vector3D(25,0,0),Vector3D(0,1,0), Vector3D(0,0,1),Vector3D(-1,0,0),120,80);
    p5->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 0, 10.0, 1, 5.0, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/newyork.ppm",0,false,0, 1.33, false, false, 0));
    objects.push_back(p5);

    s1 = new sphere(Vector3D(0, 1, s1z),2);
    s1->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 1, 1.0, 0, 15.0, NULL, 0,true,1,(1.5 -(float)mi*0.05), true, false, 0.2));
    objects.push_back(s1);
    
    s2 = new sphere(Vector3D(s2x, s2y, -4), 2);
    s2->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 1, 3.0, 0, 30.0, NULL, 0,false,0,0.8, true, false, 0));
    objects.push_back(s2);
    
    //printf("mi %d\t",mi);
    s3 = new sphere(Vector3D(s3x, s3y, -2),1);
    s3->setShader(new DSShader(1, Point3D(0,0,0), Point3D(0.2,0.2,0.2), Point3D(1,1,1), 0, 1.0, 0, 15.0, NULL, 1,false,0,0.3, true, false, 0));
    objects.push_back(s3);
    
    //transparent sphere
    s4 = new sphere(Vector3D(6, -4, 3), 1);
    s4->setShader(new DSShader(0, Point3D(0,0,1), Point3D(1,0,0), Point3D(1,1,1), 1, 1.0, 0, 15.0, NULL, 0,false,0,1.1, false, false, 0));
    //objects.push_back(s4);
    
    //environment sphere
    
    
}

void setObj(char* obj, Vector3D translate, float scale, char * texture, float kr, float kt, float eta, bool envir, bool bump, float ki)
{
    
    std::vector<Vector3D> vertices;
    std::vector<Vector2D> uvs;
    std::vector<Vector3D> normals;
    
    loadOBJ(obj, vertices, uvs, normals);
    
    for (int n = 6; n < 9; n+=3) {
        Vector3D P0 = scale*vertices[n] + translate;
        Vector3D P1 = scale*vertices[n+1] + translate;
        Vector3D P2 = scale*vertices[n+2] + translate;
        Vector3D n0, n1, n2;
        /*if (normals[n].x != NAN){
            n0 = normals[n];
            n1 = normals[n+1];
            n2 = normals[n+2];
        }else{
            printf("test %f\n", normals[n].x);
*/
            n0 = n1 = n2 =  Cross(P1 - P0, P2 - P0).Normalize();
    
        
        geometry *tri;
        tri = new triangle(P0, P1, P2, n0, n1, n2);
        tri->setShader(new DSShader(1, Point3D(1,1,0), Point3D(0.2,0,0.2), Point3D(1,0,1), 1, 10.0, 0, 15.0, texture, kr, false, kt, eta, envir, bump, ki));
        objects.push_back(tri);
        
    }
    
}

void setObjs()
{
    //setObj("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/obj/untitled.obj",Vector3D(5, 4, -1),1, NULL, 0,1, iorAnim, false, false, 0.2);
    
    //setObj("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/obj/cube.obj",Vector3D(1,5,0),2,  NULL, 0, 1, iorAnim, false, false, 0.1);
    
    setObj("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/obj/tetra.obj",Vector3D(1, -3, 0),3, NULL, 0, 1, 1, false, false, 0);
    
    //setObj("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/obj/diamond.obj",Vector3D(0, 0, 0),1, NULL, 0, 1, 0.9, true, true, 0.2);
}

void setPixels(int x_begin, int x_end, int y_begin, int y_end, int mi)
{
    for(int y = y_begin; y < y_end ; y++) {
        for(int x = x_begin; x < x_end; x++) {
            int i = (y * x_end + x) * 3;
            int maski = i;
            float maskr, maskg, maskb;
            //cout<<maski<<endl;
            maskr = (float)mask.pixmap[maski++]/255.0;
            maskg = (float)mask.pixmap[maski++]/255.0;
            maskb = (float)mask.pixmap[maski]/255.0;
            
            Vector3D maskN(0,0,0);
            maskN.Set((2*maskr-1), (2*maskg-1), 0);

            float color[] = {0.0,0.0,0.0};
            float rx = rand()/(float)RAND_MAX;
            float ry = rand()/(float)RAND_MAX;
            
            for(float i = 0; i < Num_Samples_x; i++){
                for(float j = 0; j < Num_Samples_y; j++){
                    float x_sample=(rx+i)/Num_Samples_x;
                    float y_sample=(ry+j)/Num_Samples_y;
                    Vector2D p(x+x_sample,y+y_sample);
                    float x_normalized = p.x/(x_end);
                    float y_normalized = p.y/(y_end);
                    //set Pp, npe
                    //printf("SX %f\t SY %f\n",thisScene.getSx(), thisScene.getSy());
                    Vector3D Pp = scn.getP00()
                    + (float)scn.getSx() * x_normalized * cam.getn0()
                    + (float)scn.getSy() * y_normalized * cam.getn1();
                    Vector3D npe = (Pp - cam.getPe()).Normalize();
                    npe = (npe + maskN*0.01*(float)mi).Normalize();
                    //npe = (npe + Vector3D(0.1,-0.1,0)).Normalize();
                    //depth of field
                    float DOF = 8;
                    Vector3D pointAimed = Pp + DOF * npe;
                    //pointAimed is the position of pixel on focal plane in specified ray direction and 15 is my focal length
                    
                    int DOFsample = 1;
                    float r = 0;//camera radius
                    float cDOF[] = {0, 0, 0};
                    for (int di = 0; di < DOFsample; di++){ // shooting 25 random rays
                        float du = rand()/(float)RAND_MAX;//generating random number
                        float dv = rand()/(float)RAND_MAX;
                        // creating new camera position(or ray start using jittering)
                        Vector3D newPp = Pp-(r/2)*cam.getn0()-(r/2)*cam.getn1()+r*(du)*cam.getn0() + r*(dv)*cam.getn1();
                        //printf("new Pe %f %f %f\n",Pe.x, Pe.y, Pe.z);
                        
                        //getting the new direction of ray
                        Vector3D newnpe = (pointAimed - newPp).Normalize();
                       
                        //printf("start ray tracing..");
                        //ray tracing begin
                        Point3D shadedColor(0,0,0);
                    ray r_main(newPp, newnpe);
                    intersection i = r_main.tracingMultipleObject(objects);
                    //printf("%f %f %f\n",(i.Ph - scn.getP00()).x, (i.Ph - scn.getP00()).y, (i.Ph - scn.getP00()).z);
                    Point3D cF(0,0,0);
                    if(i.intersect)
                    {
                        
                        for (int n = 0; n < (int)lights.size(); n++) {
                            cF += (1.0/((float)lights.size()))* objects[i.objectid]->getShader()->getColor(lights[n], i, r_main, objects);
                        }
                        
                        //handle color overflow
                        if (cF.x < 0 ) {
                            cF.x = 0;
                        }
                        if (cF.y < 0 ) {
                            cF.y = 0;
                        }
                        if (cF.z < 0 ) {
                            cF.z = 0;
                        }
                        if (cF.x > 1 ) {
                            cF.x = 1;
                        }
                        if (cF.y > 1 ) {
                            cF.y = 1;
                        }
                        if (cF.z > 1 ) {
                            cF.z = 1;
                        }

                        shadedColor = cF;
                    }else{
                        Point3D cB(0.6, 0.6, 0.6);
                        
                        shadedColor = cB;
                    }
                 
                    cDOF[0] += shadedColor.x;
                    cDOF[1] += shadedColor.y;
                    cDOF[2] += shadedColor.z;

                    }
                    color[0] += cDOF[0]/(float)DOFsample;
                    color[1] += cDOF[1]/(float)DOFsample;
                    color[2] += cDOF[2]/(float)DOFsample;
                    //depth of field end

                }
            }
            //sampling colors sum
            pixels[i++] = 255 * color[0] / (Num_Samples_x*Num_Samples_y);
            pixels[i++] = 255 * color[1] / (Num_Samples_x*Num_Samples_y);
            pixels[i] = 255 * color[2] / (Num_Samples_x*Num_Samples_y);
        }
    }
}

void reset()
{
    objects.clear();
//    for (int n = 0; n < W * H * 3; n++) {
//        pixels[n] = 0;
//    }

}

void call_from_thread(int tid) {
    printf( "Launched by thread %d\n", tid);
    
    setScene((float)(tid+1.0)/(float)num_threads,(num_threads - tid-1)*(1.0/2)/((float)(tid+1.0)/(float)num_threads));
    //setPixels(0, W, tid*H/num_threads, (tid+1)*H/num_threads);
    
    
    //multithreading
    
}

void windowResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void windowDisplay(void)
{
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, mask.pixmap);
    
    glutSwapBuffers();
}


void processMouse(int button, int state, int x, int y)
{
    
}


void init(void)
{
    glClearColor(1,1,1,1); // Set background color.
    glClear(GL_COLOR_BUFFER_BIT);

}

void idle()
{
    //glutPostRedisplay();
    
}

int main(int argc, char *argv[])
{
    //std::thread t[num_threads];
    //logic
    setCamera(Vector3D(30,0,0));
    setScene(1, 0);
    setLight();
    printf("scene setting completed..\n");


    //spheres initial position
    r2 = 6;
    r3 = 4;
    theta2 = 0;
    theta3 = pi / 4;
    s1z = 3;
    
    //animation
    for (int mi = 0; mi < 24; mi++)
    {
        
        setObjects(mi);
        printf("objects setting completed..\n");
        setPixels(0, W, 0, H, mi);
        printf("Ray tracing completed.\n");

        char outfile[128];
        strcpy(outfile, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/Anim2/Anim2_");
        std::string s = std::to_string(mi);
        char const *pchar = s.c_str();
        strcat(outfile, pchar);
        strcat(outfile, ".ppm");
        
        ppmview output;
        output.width = W;
        output.height = H;
        output.pixmap = pixels;
        output.writePPM(outfile, 0);

        reset();

    }
    
    //motion blur
    /*int shutter = 1;
    
    for (int mi = 0; mi < shutter; mi++)
    {
    setObjects(mi);
    //setObjs();
    printf("objects setting completed..\n");

    setPixels(0, W, 0, H);
    //printf("pixels first pixel %d %d %d\n", pixels[3], pixels[4], pixels[5]);

    printf("Ray tracing completed.\n");
        for (int n = 0; n < W * H * 3; n++) {
            MBpixels[n] += pixels[n];
        }
    reset();
    }
    
    for (int n = 0; n < W * H * 3; n++) {
        FinalPixels[n] = (unsigned char) (MBpixels[n]/(float)shutter);
        //printf("mbpixels %d\t", MBpixels[n]);
        //printf("finalpixels %d\n", FinalPixels[n]);

    }
    */
    
    

    //FinalPixels = MBpixels;
    
    /*
    int index = 0;
    for (float ior = 0.1; ior < 2.5; ior += 0.1) {
        iorAnim = ior;
        
        setObjects();
        setObjs();
        printf("objects setting completed..\n");
        setPixels(0, W, 0, H);
        printf("Ray tracing completed.\n");
        printf("writing to ppm files %d ...", index);
        
        char outfile[128];
        strcpy(outfile, "/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/Pr6/IORanimation/ior_");
        std::string s = std::to_string(index);
        char const *fileIndex = s.c_str();  //use char const* as target type
        strcat(outfile,fileIndex);
        strcat(outfile, ".ppm");
        
        ppmview output;
        output.width = W;
        output.height = H;
        output.pixmap = pixels;
        output.writePPM(outfile, 0);
        printf("Finished writing image %d\n", index);
        index++;
        reset();
    }
    */
    
    //    //Launch a group of threads
    //    for (int tid = 0; tid < num_threads; ++tid) {
    //        t[tid] = std::thread(call_from_thread, tid);
    //    }
    //
    //    std::cout << "Launched from the main\n";
    //
    //    //Join the threads with the main thread
    //    for (int i = 0; i < num_threads; ++i) {
    //        t[i].join();
    //    }
    //UI
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    
    glutInitWindowSize(W, H);
    
    glutCreateWindow("Ray Tracer");
    init();
    
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    
    glutMouseFunc(processMouse);
    glutIdleFunc(idle);

    
    
    glutMainLoop();
    
    return 0;
}
