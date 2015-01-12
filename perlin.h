//
//  perlinNoise.h
//  RayTracer
//
//  Created by Uriana on 4/9/14.
//  Copyright (c) 2014 Uriana. All rights reserved.
//

#ifndef __RayTracer__perlinNoise__
#define __RayTracer__perlinNoise__

#include <iostream>

class perlin
{
public:
    perlin(void);
    double noise(double x, double y, double z);
    
private:
    int p[512];
    static perlin & getInstance(){static perlin instance; return
        instance;}
    static double fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    
    static double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }
    
    static double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        // CONVERT LO 4 BITS OF HASH CODE
        double u = h<8||h==12||h==13 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4||h == 12||h == 13 ? y : z;
        return ((h & 1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    }
};
#endif /* defined(__RayTracer__perlinNoise__) */
