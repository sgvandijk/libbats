#include "math.ih"

bool Math::intersectLineCircle(Vector3d& intersection, Vector3d const& position, Vector3d const& direction, Vector3d const& center, double radius)
{
  Vector3d pcdif = position - center;
  double a = direction.squaredNorm();
  double b = 2 * direction.dot(pcdif);
  double c = center.squaredNorm() + position.squaredNorm() - 2 * (center.dot(position)) - radius * radius;
  
  double d = b * b - 4 * a * c;
  if (d < 0)
    return false;
  else if (d == 0)
  {
    double lambda = -b / (2 * a);
    intersection = position + lambda * direction;
    return true;
  }
  else
  {
    double lambda1 = (-b + sqrt(d))/(2 * a);
    double lambda2 = (-b - sqrt(d))/(2 * a);
    intersection = fabs(lambda1) < fabs(lambda2) ? position + lambda1 * direction : position + lambda2 * direction;
    return true;
  }
}
