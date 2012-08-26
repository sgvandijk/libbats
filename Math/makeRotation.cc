#include "math.ih"

Affine3d Math::makeRotation(Vector3d const angles)
{

  return Eigen::Affine3d(Eigen::AngleAxisd(angles(0), Eigen::Vector3d::UnitX()) *
			    Eigen::AngleAxisd(angles(1), Eigen::Vector3d::UnitY()) *
			    Eigen::AngleAxisd(angles(2), Eigen::Vector3d::UnitZ()));

  // x = phi, y = theta, z = psi
/*
  double cx = cos(angles.x());
  double cy = cos(angles.y());
  double cz = cos(angles.z());
  double sx = sin(angles.x());
  double sy = sin(angles.y());
  double sz = sin(angles.z());
  Eigen::Affine3d rot;
  rot.matrix() <<
    (cz * cy),  (cz * sy * sx - cx * sz),  (sz * sx + cz * cx * sy), 0,
    (cy * sz),  (cz * cx + sz * sy * sx),  (cx * sz * sy - cz * sx), 0,
    -(sy)    ,  (cy * sx)               ,  (cy * cx)               , 0,
    0        ,  0                       ,  0                       , 1;

  return rot;
*/
}
