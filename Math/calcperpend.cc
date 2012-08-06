#include "math.ih"

/**
  *  Calculates the perpendicular vector to v.
  */
Vector3d Math::calcPerpend(Vector3d const &v)
{
  /*
  if (!(v.y() < 0.00001 && v.y() > -0.00001) && !(v.x() < 0.00001 && v.x() > -0.00001))
    return Vector3d(1.0, -1.0/(v.y()/v.x()), 0.0).normalize();
  
  if (v.x() < 0.00001 && v.x() > -0.00001)
    return Vector3d(1.0, 0.0, 0.0);
    
  return Vector3d(0.0, 1.0, 0.0);*/
  
  Vector3d up(0,0,1);
  return v.cross(up);
}

