#-------------------------------------------------
#
# Project created by QtCreator 2014-02-04T21:19:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayTracer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    scene.cpp \
    Vector2D.cpp \
    Vector3D.cpp \
    pixmap.cpp \
    geometry.cpp \
    sphere.cpp \
    plane.cpp \
    light.cpp \
    shader.cpp \
    ray.cpp \
    quadric.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    scene.h \
    Vector2D.h \
    Vector3D.h \
    pixmap.h \
    geometry.h \
    sphere.h \
    plane.h \
    light.h \
    shader.h \
    ray.h \
    quadric.h

FORMS    += mainwindow.ui

LIBS += -framework glut
