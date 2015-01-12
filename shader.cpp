#include "shader.h"
#include "ray.h"
#include "geometry.h"

shader::shader(float alpha, Point3D c0, Point3D c1, Point3D c2, int dMode, float da, int sMode, float sa, char * textureImage, float kr, bool glossy, float kt, float eta, bool environmentalMapping, bool bumpMapping, float ki )
{
    this->alpha = alpha;
    this->dMode = dMode;
    this->sMode = sMode;
    this->da = da;
    this->sa = sa;
    this->c0 = c0;
    this->c1 = c1;
    this->c2 = c2;
    if (textureImage != NULL) {
        tex.loadImage(textureImage);
        tex.hasTex = true;
    }
    this->kr = kr;
    this->glossy = glossy;
    this->kt = kt;
    
    eta = 1/eta;
    this->eta = eta;

    this->ki = ki;
    this->environmentalMapping = environmentalMapping;
    if (environmentalMapping) {
        environmentalMap.loadImage("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/Diamond.ppm");
    }
    this->bumpMapping = bumpMapping;
    if (bumpMapping) {
        bumpMap.loadImage("/Users/Uriana/Dropbox/TAMU/Courses/ImageSynthesis/RayTracer/RayTracer/textures/bumpRock.ppm");
        printf("bump map w %d h %d", bumpMap.width, bumpMap.height);
    }
}

void shader::init(light *l, intersection i, Vector3D nr)
{
    Vector3D PL, NL, Ph, nh;
    Point3D CL;
    
    Ph = i.Ph;
    nh = i.nh;
    CL = l->getColor();
    
    switch (l->lightType()) {
        case 0:
            //point light
            PL = l->getPL();
            //printf("Ph %f %f %f\n", Ph.x, Ph.y, Ph.z);
            NL = -(PL - Ph).Normalize();
            break;
        case 1:
            //directional light
            NL = -(l->getnL().Normalize());
            break;
        case 2:
            //spot light
            PL = l->getPL();
            NL = - (l->getnL().Normalize());
            if (Dot((Ph - PL).Normalize(), -NL) <= cosf(l->getTheta())) {
                CL = Point3D(0.1,0.1,0.1);
            }
            break;
            
        default:
            break;
    }
    
    
    thisPL = PL;
    thisCL = CL;
    thisNL = NL;
    thisPh = Ph;
    thisnh = nh;
    thisnpe = nr;
    
    diffuseShading(dMode, da);
    specularShading(sMode, sa);
}

float shader::getTransparency()
{
    return alpha;
}

Point3D shader::MultiplyColor(Point3D a, Point3D b)
{
    Point3D c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    return c;
}

float shader::calCosTheta(int mode, float a, Vector3D v, Vector3D n)
{
    float cosTheta = Dot(v,n);
    float c = 0;
    //switch shading mode
    //0 -  c = cos(theta)
    //1 -  c = (cos(theta) + 1)/2
    switch(mode)
    {
        case 0:
            c = cosTheta;
            if (c < 0) c = 0;
            break;
        case 1:
            c = (cosTheta + 1)/2.0;
            break;
        default:
            c = cosTheta;
            break;
    }
    
    //control color distribution
    c = powf(c,a);
    return c;
}

void shader::diffuseShading(int mode, float a)
{
    //switch shading mode
    //0 - Lambert shading | c = cos(theta)
    //1 - Gooch shading | c = (cos(theta) + 1)/2
    c = calCosTheta(mode, a, thisnh, thisNL);
    
}

Vector2D shader::findNormals(Vector2D &v1, Vector2D &v2)
{
    Vector2D v12 = v1-v2;
    
    v12.Rotate(-90);
    v12.Normalize();
    return v12;
}
void shader::handleColorOverflow(Point3D &color){
    if (color.x < 0) {
        color.x = 0;
    }
    if (color.y < 0) {
        color.y = 0;
    }
    if (color.z < 0) {
        color.z = 0;
    }
    if (color.x > 1) {
        color.x = 1;
    }
    if (color.y > 1) {
        color.y = 1;
    }
    if (color.z > 1) {
        color.z = 1;
    }
}

void shader::specularShading(int mode, float a)
{
    r = thisnpe - 2 * Dot(thisnh, thisnpe) * thisnh;
    
    
    //switch shading mode
    //0 - Sharp Circular Specular
    
    switch(mode)
    {
        case 0:
        {
            if (c >= 0) {
                s = calCosTheta(1, a, r, -thisNL);
                if (s < 0.01) s = 0;
            }
            break;
        }
        case 1:
            s = 0;
            break;
            
            /*    case 1:
             {
             if (rl.i.hasIntersect) {
             Vector3D P00 = thisPL - thisL.getS0()/2*n0 - thisL.getS1()/2*n1;
             float x = Dot((rl.i.Ph - P00),n0) /thisL.getS0();
             float y = Dot((rl.i.Ph - P00),n1) /thisL.getS1();
             if(x>=0 && x<=1 && y>=0 && y<=1)
             s = 1;
             else
             s = 0;
             }
             break;
             }
             case 2:
             {
             
             if (rl.i.hasIntersect) {
             float x = 2/thisL.getS0() * Dot((rl.i.Ph - thisPL), n0);
             float y = 2/thisL.getS1() * Dot((rl.i.Ph - thisPL), n1);
             if(fabs(x)>= 0.2 && fabs(x) <=1 && fabs(y)>= 0.2 && fabs(y) <=1)
             s = 1;
             else
             s = 0;
             }
             break;
             }
             case 3:
             {
             Vector2D p0(300.0/640, 370.0/480);
             Vector2D p1(190.0/640, 260.0/480);
             Vector2D p2(230.0/640, 170.0/480);
             Vector2D p3(404.0/640, 180.0/480);
             Vector2D p4(390.0/640, 330.0/480);
             
             Vector2D p02 = findNormals(p2,p0);
             Vector2D p13 = findNormals(p3,p1);
             Vector2D p24 = findNormals(p4,p2);
             Vector2D p30 = findNormals(p0,p3);
             Vector2D p41 = findNormals(p1,p4);
             
             if (rl.i.hasIntersect) {
             float x = 2/thisL.getS0() * Dot((rl.i.Ph - thisPL), n0);
             float y = 2/thisL.getS1() * Dot((rl.i.Ph - thisPL), n1);
             int flag = 0;
             Vector2D p(x,y);
             if (Dot(p02,(p-p0)) <= 0)
             flag +=1;
             if (Dot(p13,(p-p1)) <= 0)
             flag +=1;
             if (Dot(p24,(p-p2)) <= 0)
             flag +=1;
             if (Dot(p30,(p-p3)) <= 0)
             flag +=1;
             if (Dot(p41,(p-p4)) <= 0)
             flag +=1;
             
             if(flag >= 4)
             s = 1;
             else
             s = 0;
             }
             }
             */
        default:
            break;
    }
    
    
}
void shader::calUV(Vector3D thisraynh, float &u, float &v)
{
    //calculating uv based on ray
    float phi, theta;
    float cosPhi = thisraynh.z;
    if (cosPhi < -1) {
        cosPhi = -1;
    }
    if (cosPhi > 1) {
        cosPhi = 1;
    }
    
    phi = pi - acos(cosPhi);
    
    float cosTheta = thisraynh.y / sinf(phi);
    if (cosTheta < -1) {
        cosTheta = -1;
    }
    if (cosTheta > 1) {
        cosTheta = 1;
    }
    
    theta = asin(cosTheta) + pi;
    if (theta < 0 ) {
        theta += pi;
    }
    if (theta > 2*pi) {
        theta -= 2*pi;
    }
    
    //range from 0 to 1
    u = theta / (2 * pi);
    v = phi / pi;
    
}
void shader::findCoordinateAt(geometry* sphere, intersection i, float phiX, float  thetaX, float phiY, float thetaY, Vector3D &Nx, Vector3D &Ny){
    
    Vector3D Phx, Phy;
    Phx.Set(sphere->getr() * sinf(thetaX) * sinf(phiX) + sphere->getPc().x, sphere->getr() * cosf(thetaX) * sinf(phiX) + sphere->getPc().y, sphere->getr() * cosf(phiX) + sphere->getPc().z);
    Phy.Set(sphere->getr() * sinf(thetaY) * sinf(phiY) + sphere->getPc().x, sphere->getr() * cosf(thetaY) * sinf(phiY) + sphere->getPc().y, sphere->getr() * cosf(phiY) + sphere->getPc().z);
    
    Nx = (Phx - i.Ph).Normalize();
    Ny = (Phy - i.Ph).Normalize();

}

void shader::doBumpMapping(geometry* object, intersection &i){
    Point3D bumpColor = bumpMap.getColorAt(i.u, i.v);
    float nx, ny, nz;
    nx = 2 * bumpColor.x - 1;
    ny = 2 * bumpColor.y - 1;
    nz = bumpColor.z;
    
    float phiX, thetaX, phiY, thetaY;
    thetaX = ((int)(i.u * bumpMap.width) + 1) / (float)bumpMap.width * 2 * pi;
    phiX = i.v * pi;
    thetaY = i.u * 2 * pi;
    phiY = ((int)(i.v * bumpMap.height) + 1) / (float)bumpMap.height * pi;
    
    
    Vector3D Nx, Ny;
    if (object->objectType() == 1) {
        //is sphere
        findCoordinateAt(object,i, phiX, thetaX, phiY, thetaY, Nx, Ny);
        
    }else {
        //is plane or triangle
        Nx = object->getn0();
        Ny = object->getn1();
    }
    //printf("previous nh %f %f %f\t", i.nh.x, i.nh.y, i.nh.z);

    i.nh = (i.nh * nz + Nx * nx + Ny * ny).Normalize();
    //printf("nh %f %f %f\n", i.nh.x, i.nh.y, i.nh.z);

}

Point3D shader::doRefraction(ray thisray, std::vector<geometry*> objects, intersection i, light* l, float u, float v)
{

    Point3D color(0,0,0);
    if (thisray.getBounces() > 0 && kt != 0 ) {

    float localEta = eta;


    //check reflection or refraction
    Vector3D tv = - thisray.getnr().Normalize();
    Vector3D tn = i.nh.Normalize();
    float C = Dot(tv, tn);
    
    if (C < 0) {
        //printf("c?????");
        tn = -tn;
        localEta = 1.0 / localEta;
    }
    
    float term = (C*C- 1) / (localEta*localEta) + 1;

    Vector3D t;
    if (term >= 0) {
        
        //printf("refraction occured? %f\n", localEta);
        //refraction
        t = (-1.0 / localEta) * tv + (C / localEta - sqrtf(term)) * tn;
        //printf("npe %f %f %f\t", thisray.getnr().x, thisray.getnr().y, thisray.getnr().z);
        //printf("t %f %f %f\n", t.x, t.y, t.z);
        //printf("localEta %f tv %f %f %f C %f term %f tn %f %f %f\n", localEta, tv.x, tv.y, tv.z, C, term, tn.x, tn.y, tn.z);
        //printf(" (C / localEta - sqrtf(term)) %f %f tn %f %f %f\n",  C, sqrtf(term), tn.x, tn.y, tn.z);
    }else{
        //do reflection when eta is large enough
        t = - tv + 2.0 * C * tn;
    }

        int translucentSample = 10;
        Point3D colorTs(0,0,0);
        for (int ti = 0; ti < translucentSample; ti++) {
            Point3D colorT;
            Vector3D newt(0,0,0);
            float rotateAngle;
            rotateAngle = acos(Dot(thisnh, r));
            Vector3D rotateAxis;
            rotateAxis = Cross(thisnh, r).Normalize();
            
            Vector3D newtn0, newtn1;
            Vector3D n0, n1;
            
            float phiX, thetaX, phiY, thetaY;
            thetaX = ((int)(i.u * 100) + 1) / (float)100 * 2 * pi;
            phiX = i.v * pi;
            thetaY = i.u * 2 * pi;
            phiY = ((int)(i.v * 100) + 1) / (float)100 * pi;
            findCoordinateAt(objects[i.objectid],i, phiX, thetaX, phiY, thetaY, n0, n1);
            newtn0 = n0.RotateAboutAxis(rotateAngle, rotateAxis);
            newtn1 = Cross(r, newtn0).Normalize();
            float rx, ry;
            rx = 0.02 * rand()/(float)RAND_MAX;
            ry = 0.02 * rand()/(float)RAND_MAX;
            
            newt = t + rx * newtn0 + ry * newtn1;
            //printf("new r %f %f %f r %f %f %f\n",newt.x, newt.y, newt.z, r.x, r.y, r.z);
            if (term < 0) {
                newt = t;
                //printf("here");
            }


    ray r_refract(thisPh, newt.Normalize());
        r_refract.setBounces(thisray.getBounces() - 1);
        std::vector<geometry *> otherObjects;
        otherObjects = objects;
        if (otherObjects.size() != 1)
            otherObjects.erase(otherObjects.begin() + i.objectid);
        
        intersection intersect = r_refract.tracingMultipleObject(otherObjects);
        if (intersect.intersect) {
            //printf("hit object %d\n", otherObjects[intersect.objectid]->objectType());
            colorT = otherObjects[intersect.objectid]->getShader()->getColor(l, intersect, r_refract, otherObjects);
            
            //printf("intersect.objectid %d colorT %f %f %f \n", intersect.objectid,colorT.x, colorT.y, colorT.z);
        }else{
            //environment map
            if (environmentalMapping) {
                if (environmentalMap.width > 2 * environmentalMap.height) {
                    environmentalMap.width = 2 * environmentalMap.height;
                }else {
                    environmentalMap.height = 1/2.0 * environmentalMap.width;
                }
                
                colorT = environmentalMap.getColorAt(u, v);
                
            }else{
                colorT = Point3D(0, 0, 0);
            }
            
        }
            colorTs += colorT;
        }
        color = colorTs / (float)translucentSample;
    }
    handleColorOverflow(color);
    return color;
}

Point3D shader::doReflection(ray thisray, Vector3D r,std::vector<geometry*> objects, int objectId, light* l, float u, float v){
    Point3D colorR(0,0,0);
    if ( thisray.getBounces() > 0 ) {
        ray r_reflect(thisPh, r.Normalize());
        r_reflect.setBounces(thisray.getBounces() - 1);
        
        std::vector<geometry *> otherObjects;
        otherObjects = objects;
        if (otherObjects.size() != 1)
            otherObjects.erase(otherObjects.begin() + objectId);
        intersection intersect = r_reflect.tracingMultipleObject(otherObjects);
        
        if (intersect.intersect) {
            colorR = otherObjects[intersect.objectid]->getShader()->getColor(l, intersect, r_reflect, otherObjects);
        }else{
            //environment map
            if (environmentalMapping) {
                if (environmentalMap.width > 2 * environmentalMap.height) {
                    environmentalMap.width = 2 * environmentalMap.height;
                }else {
                    environmentalMap.height = 1/2.0 * environmentalMap.width;
                }
                
                colorR = environmentalMap.getColorAt(u, v);
                
            }else{
                colorR = Point3D(0, 0, 0);
            }
            
        }
        
    }
    
    return colorR;
}

Point3D shader::doReflection(ray thisray,std::vector<geometry*> objects, int objectId, light* l, float u, float v){
    Point3D colorR(0,0,0);
    if ( thisray.getBounces() > 0 ) {
        ray r_reflect(thisPh, r.Normalize());
        r_reflect.setBounces(thisray.getBounces() - 1);
        
        std::vector<geometry *> otherObjects;
        otherObjects = objects;
        if (otherObjects.size() != 1)
            otherObjects.erase(otherObjects.begin() + objectId);
        intersection intersect = r_reflect.tracingMultipleObject(otherObjects);
        
        if (intersect.intersect) {
            colorR = otherObjects[intersect.objectid]->getShader()->getColor(l, intersect, r_reflect, otherObjects);
        }else{
            //environment map
            if (environmentalMapping) {
                if (environmentalMap.width > 2 * environmentalMap.height) {
                    environmentalMap.width = 2 * environmentalMap.height;
                }else {
                    environmentalMap.height = 1/2.0 * environmentalMap.width;
                }
                
                colorR = environmentalMap.getColorAt(u, v);
                
            }else{
                colorR = Point3D(0, 0, 0);
            }
            
        }
        
    }
    
    return colorR;
}

Point3D shader::doIridescence()
{
    Point3D colorI;
    float t = 1 - powf(c, 2.3);
    if (t > 1) {
        t = 1;
    }
    float colorH = t * 720;
    if (colorH > 360) {
        colorH-= 360;
    }
    float colorS = 1;
    float colorV = (sinf( 6 / t * 2 * pi) + 1 ) / 4.0 + 0.2;
    if (colorV > 1) {
        colorV = 1;
    }
    float colorR, colorG, colorB;
    colorSpaceConversion colorConvert;
    colorConvert.HSVtoRGB(&colorR, &colorG, &colorB, colorH, colorS, colorV);
    colorI.Set(colorR, colorG, colorB);
    handleColorOverflow(colorI);
    return colorI;

}

float shader::doShadowCasting( std::vector<geometry *> objects, int objectId){
    //calculating shadow
    ray r_shadow(thisPh, (thisPL - thisPh).Normalize());
    float d = Magnitude(thisPL - thisPh);
    float ks;
    std::vector<geometry *> otherObjects;
    otherObjects = objects;
        otherObjects.erase(otherObjects.begin() + objectId);
        std::vector<intersection> in_shadow = r_shadow.tracingMultipleObjectGetAll(otherObjects);
        
        float C1[(int)in_shadow.size()];
        float C1Total = 1.0;
        
        for ( int m = 0; m < (int)in_shadow.size(); m++) {
            if (in_shadow[m].intersect && in_shadow[m].th < d) {
                C1[m] = (Dot( r_shadow.getnr(), in_shadow[m].nh) + 1) * (Dot(thisnh, r_shadow.getnr()) + 1)/2;
                C1Total *= C1[m];

            }
        }

        ks = C1Total;
    
    return ks;
}

Point3D DSShader::getCd(intersection i){
    return colorD;
}

Point3D DSShader::getCs(){
    colorS = c2;
    return colorS;
}

Point3D DSShader::getColor(light *l, intersection i, ray thisray, std::vector<geometry*> objects)
{
    init(l, i, thisray.getnr());
    float u, v;
    calUV(r, u, v);

    //bump map
    //altering the normal
    if (bumpMapping) {
        doBumpMapping(objects[i.objectid], i);
        //calculating cos theta
        init(l, i, thisray.getnr());
        //calculating uv based on new reflection ray
        calUV(r, u, v);
        
    }
    
    
    //apply texture color
    if (tex.hasTex) {
        c0 = tex.getColorAt(i.u, i.v);
        handleColorOverflow(c0);
    }
    
    // do environment mapping
    if (environmentalMapping && kr == 0 && kt == 0) {
        if (environmentalMap.width > 2 * environmentalMap.height) {
            environmentalMap.width = 2 * environmentalMap.height;
        }else {
            environmentalMap.height = 1/2.0 * environmentalMap.width;
        }
        
        colorD = environmentalMap.getColorAt(u, v);
        
    }else{
        colorD = (1 - c) * c0 + c * c1;
    }
    
    colorS = c2;
    handleColorOverflow(colorD);
    handleColorOverflow(colorS);
    
    Point3D Cds;
    
    //Gooch compositing
    Cds = colorD * (1 - s) + colorS * s;
    
    //shadow casting
    float kshadow = doShadowCasting(objects, i.objectid);
    Cds = Cds * kshadow;
    
    Point3D Cdsr;
    //do reflection
    //r = thisnpe - 2 * Dot(thisnh, thisnpe) * thisnh;
    if (kr != 0) {
        if (glossy) {
        int glossySample = 10;
        for (int gi = 0; gi < glossySample; gi++) {
            Vector3D newr(0,0,0);
            float rotateAngle;
            rotateAngle = acos(Dot(thisnh, r));
            Vector3D rotateAxis;
            rotateAxis = Cross(thisnh, r).Normalize();
            
            Vector3D newrn0, newrn1;
            Vector3D n0, n1;
            
            float phiX, thetaX, phiY, thetaY;
            thetaX = ((int)(i.u * 100) + 1) / (float)100 * 2 * pi;
            phiX = i.v * pi;
            thetaY = i.u * 2 * pi;
            phiY = ((int)(i.v * 100) + 1) / (float)100 * pi;
            findCoordinateAt(objects[i.objectid],i, phiX, thetaX, phiY, thetaY, n0, n1);
            newrn0 = n0.RotateAboutAxis(rotateAngle, rotateAxis);
            newrn1 = Cross(r, newrn0).Normalize();
            float rx, ry;
            rx = 0.2 * rand()/(float)RAND_MAX;
            ry = 0.2 * rand()/(float)RAND_MAX;

            newr = r + rx * newrn0 + ry * newrn1;
            //printf("new r %f %f %f r %f %f %f\n",newr.x, newr.y, newr.z, r.x, r.y, r.z);

            Cdsr += Cds * (1 - kr) + kr * doReflection(thisray, newr, objects, i.objectid, l, u, v);
        }
        Cdsr = Cdsr/(float)glossySample;
        }else{
            Cdsr = Cds * (1 - kr) + kr * doReflection(thisray, objects, i.objectid, l, u, v);
        }
    }else{
        Cdsr = Cds;
    }

    Point3D Cdsrt;
    //refraction
    if (kt != 0) {
        //normal map eta
        if (tex.hasTex) {
            eta = (tex.getColorAt(i.u, i.v).x + tex.getColorAt(i.u, i.v).y + tex.getColorAt(i.u, i.v).z)/3.0;
        }
        
        Cdsrt = Cdsr * (1 - kt) + kt *doRefraction(thisray, objects, i, l, u, v);

    }else{
        Cdsrt = Cdsr;
    }
    

    color = MultiplyColor(thisCL, Cdsrt);
    handleColorOverflow(color);
    
    
    //apply iridescence effect
    if (ki != 0) {
        Point3D colorI;
        colorI = doIridescence();
        color = color * (1-ki) + colorI * ki;
    }

    return color;
}



Point3D phongShader::getColor(light *l, intersection i, ray thisray, std::vector<geometry*> objects)
{
    init(l, i, thisray.getnr());
    
    kd = 0.5;
    ks = 0.3;
    ka = 0.2;
    
    if (tex.hasTex) {
        c0 = tex.getColorAt(i.u, i.v);
    }
    
    //classical
    color = (kd * c )* c0 + (ks * s) * c1 + ka * c2;
    color = MultiplyColor(thisCL, color);
    
    if (color.x > 1) {
        color.x = 1;
    }
    if (color.y > 1) {
        color.y = 1;
    }
    if (color.z > 1) {
        color.z = 1;
    }
    
    
    return color;
}
