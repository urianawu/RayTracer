#include "geometry.h"

geometry::geometry()
{
}

void geometry::setShader(shader *shader)
{
    s = shader;
}
shader* geometry::getShader()
{
    return s;
}