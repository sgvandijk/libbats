#include "math.ih"

Vector3d Math::linePointClosestToPoint(Vector3d const &l0,
				 Vector3d const &lVect,
				 Vector3d const &point,
         bool segment)
{
  Vector3d v = lVect.normalized();
  
  Vector3d s = calcPerpend(v);

  // There probably is a more efficient formula.
  double u;
  if (s.y() == 0)
    u = (l0.y() - point.y()) / -v.y();
  else
    u = ((s.x() / s.y())*(l0.y() - point.y()) + (point.x() - l0.x()))/(v.x() - (s.x() / s.y()) * v.y());

  // When no perpendicular line is posible within the segment, use
  // the closest endpoint.
  if (segment && (u > lVect.norm() || u < 0)) {
    if ((l0 - point).norm() < ((l0 + lVect) - point).norm())
      return l0;
    else
      return l0+lVect;
  }

  return l0 + v*u;
}

