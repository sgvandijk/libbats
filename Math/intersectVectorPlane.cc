#include "math.ih"


Vector3d Math::intersectVectorPlane(Vector3d const& position, Vector3d const& direction, Vector4d const& plane)
{
  // x = pos.x + eta * dir.x
  // a * x + b * y + c * z + d = 0
  // a * pos.x + a * eta * dir.x +
  // b * pos.y + b * eta * dir.y +
  // c * pos.z + c * eta * dir.z +
  // d = 0;
  // eta * (a * dir.x + b * dir.y + c * dir.z) = - a * pos.x - b * pos.y - c * pos.z - d
  // eta = (- a * pos.x - b * pos.y - c * pos.z - d) / (a * dir.x + b * dir.y + c * dir.z)
  
  double eta = - (plane(0) * position.x() + plane(1) * position.y() + plane(2) * position.z() + plane(3)) /
                 (plane(0) * direction.x() + plane(1) * direction.y() + plane(2) * direction.z());
  return position + direction * eta;
}
