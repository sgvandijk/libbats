#include "math.ih"

/**
  *  Calculates the distance to the closest point
  *  on a line.
  *
  *  @param l0 the starting point of the line.
  *  @param lVect the normalized vector of the line.
  *  @param The point.
  */
double Math::distanceLinePoint(Vector3d const &l0,
			 Vector3d const &lVect,
			 Vector3d const &point)
{
  Vector3d v = lVect.normalized();
  Vector3d s = calcPerpend(v);
  
  if (s(1) == 0)
    return fabs(point(0) - l0(0));
    
  double u = ((s[0] / s[1]) * (l0.y() - point.y()) +
              (point.x() - l0.x())) / (v[0] - (s[0]/s[1])*v[1]);

  return (l0 + lVect.normalized()*u - point).norm();
}

