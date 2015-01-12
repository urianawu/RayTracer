//
//  texture.cpp
//  RayTracer
//
//  Created by Uriana on 4/2/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#include "texture.h"
void texture::loadImage(char * filename)
{
    textureImage.readPPM (filename);
    width = textureImage.width;
    height = textureImage.height;
    pixmap = textureImage.pixmap;
}

Point3D texture::getColorAt(float u, float v)
{
    //nearest pixel color
    int I, J;
    float X, Y;
    X = u * (width-1.0);
    Y = v * (height-1.0);
    I = (int)X;
    J = (int)Y;
    float i, j;
    i = X - (float)I;
    j = Y - (float)J;
    int maxPixelIndex = (height * width + width) * 3;
    int pixelIndex00 = (J * width + I) * 3;
    int pixelIndex01 = ((J + 1) * width + I) * 3;
    int pixelIndex10 = (J * width + I + 1) * 3;
    int pixelIndex11 = ((J + 1) * width + I + 1) * 3;
    if (pixelIndex00 > maxPixelIndex) {
        pixelIndex00 = maxPixelIndex;
    }
    if (pixelIndex01 > maxPixelIndex) {
        pixelIndex01 = maxPixelIndex;
    }
    if (pixelIndex10 > maxPixelIndex) {
        pixelIndex10 = maxPixelIndex;
    }
    if (pixelIndex11 > maxPixelIndex) {
        pixelIndex11 = maxPixelIndex;
    }
    
    Point3D color;
    color.x = (pixmap[pixelIndex00++] * (1-i) * (1-j)
               + pixmap[pixelIndex10++] * i * (1-j)
               + pixmap[pixelIndex11++] * i * j
               + pixmap[pixelIndex01++] * (1-i) * j)/ 255.0;
    color.y = (pixmap[pixelIndex00++] * (1-i) * (1-j)
    + pixmap[pixelIndex10++] * i * (1-j)
    + pixmap[pixelIndex11++] * i * j
    + pixmap[pixelIndex01++] * (1-i) * j) / 255.0;
    color.z = (pixmap[pixelIndex00] * (1-i) * (1-j)
    + pixmap[pixelIndex10] * i * (1-j)
    + pixmap[pixelIndex11] * i * j
    + pixmap[pixelIndex01] * (1-i) * j) / 255.0;
    
    return color;
}

