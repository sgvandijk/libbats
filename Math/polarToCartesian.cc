#include "math.ih"


Vector3d Math::polarToCartesian(Vector3d const &polar)
{
  Vector3d cartesian;
	
  cartesian.z() = (sin(polar.z()) * polar.x());
  double dmark = cos(polar.z()) * polar.x();
  cartesian.y() = (cos(-polar.y()) * dmark);
  cartesian.x() = (sin(-polar.y()) * dmark);
  
  return cartesian;
}

/*Vector3d Math::polarToCartesian2(Vector3d const& polar)
{
  Vector3d cartesian;
  
  double dist = polar.x();
  double tanTheta = tan(polar.y());
  double tanPhi = tan(polar.z());

  cartesian.x() = (sqrt(dist * dist / (tanTheta * tanTheta + tanPhi * tanPhi + 1)));
  if (fabs(polar.y()) > .5 * M_PI)
	  cartesian[0] = -cartesian[0];
  
  cartesian.y() = (cartesian.x() * tanTheta);
  cartesian.z() = (cartesian.x() * tanPhi);
  
  return cartesian;
}*/
