#include "glwidget.h"
#include <glut/glut.h>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer,SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}
void GLWidget::initializeGL(){
    glClearColor(1,1,1,1); // Set background color.
}

void GLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glutSwapBuffers();

}

void GLWidget::resizeGL(int w, int h){

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
