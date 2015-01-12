#include "ray.h"

ray::ray(Vector3D thePr, Vector3D thenr)
{
    Pr = thePr;
    nr = thenr;
}
Vector3D ray::getPr()
{
    return Pr;
}

Vector3D ray::getnr()
{
    return nr;
}

void ray::shoot(geometry *g, int type)
{
    i.hasIntersect = false;
    switch(type)
    {
    case 0:
    {
        float c = Dot((g->getPc() - Pr),(g->getPc() - Pr)) - powf(g->getr(),2);
        if (c < 0) {
            printf("\nEye point is inside the sphere\n");
            exit(0);
        }
        float b = Dot(nr, (g->getPc() - Pr));
        float delta = powf(b,2) - c;
        if ( b >= 0 && delta >= 0){
            //intersection occurs
            float th = b - sqrtf(delta);
            i.th = th;
            i.Ph = Pr + nr * th;
            i.nh = (i.Ph - g->getPc())/g->getr();
            i.hasIntersect = true;
        }
        break;
    }
    case 1:
    {
        float th = (Dot(g->getn0(),g->getP0()) - Dot(g->getn0(),Pr)) / Dot(g->getn0(),nr);
        if (th > 0 && Dot(g->getn0(),nr) != 0) {
        i.Ph = Pr + nr *th;
        i.nh = g->getn0();
        i.th = th;
        i.hasIntersect = true;
        }
        break;
    }
    default:
        break;
    }


}


