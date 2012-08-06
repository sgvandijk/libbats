#include "math.ih"


Vector3d Math::intersectLines2D(Vector3d const& pos1, Vector3d const& dir1, Vector3d const& pos2, Vector3d const& dir2)
{

  double y = (pos2.y() - pos1.y()) / dir1.y();
  double x = (pos2.x() - pos1.x()) / dir1.x();
  double a = dir2.x() / dir1.x() - dir2.y() / dir1.y();
  double lambda2 = (y - x) / a;
  
  Vector3d intersect = pos2 + dir2 * lambda2;
  return intersect;
}

