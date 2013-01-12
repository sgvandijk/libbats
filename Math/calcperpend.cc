#include "math.ih"

Vector3d Math::calcPerpend(Vector3d const &v)
{
  Vector3d up(0,0,1);
  return v.cross(up);
}

