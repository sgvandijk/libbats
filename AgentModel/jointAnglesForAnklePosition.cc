#include "agentmodel.ih"

VectorXd AgentModel::jointAnglesForAnklePosition(Vector3d const& p, Types::Side side, double j1Angle)
{
  VectorXd res = VectorXd::Zero(6);
 
  // lines between hip-knee and knee-ankle
  // TODO: this can be static
  VectorXd link1 = getJoint(Types::LLEG2)->anchors.second - getJoint(Types::LLEG4)->anchors.first;
  VectorXd link2 = getJoint(Types::LLEG4)->anchors.second - getJoint(Types::LLEG5)->anchors.first;

  // Offset due to knee not being on 0-degree line from hip
  double j2offset = atan2(link1.y(), link1.z());
  double j4offset = atan2(link2.y(), link2.z());

  double l1sq = link1.squaredNorm();
  double l2sq = link2.squaredNorm();

  // Lengths of the links
  double l1 = sqrt(l1sq);
  double l2 = sqrt(l2sq);
  
  // Transform to incorporate joint1 angle
  Vector3d axis = getJoint(side == Types::LEFT ? Types::LLEG1 : Types::RLEG1)->axis;
  Vector3d pos = AngleAxisd(-j1Angle, axis) * p;

  // Too far away
  if (pos.norm() > (l1 + l2))
  {
    pos = pos.normalized() * (l1 + l2);
    cerr << "(AgentModel::jointAnglesForAnklePos) Invalid target position: too far!" << endl;
  }
  
  // First sideways
  res(Types::LLEG3 - Types::LLEG1) = -atan2(pos.x(), -pos.z());
  
  // Transform pos 
  Affine3d rot(Math::makeRotation(Vector3d(0, -res(Types::LLEG3 - Types::LLEG1), 0)));
  Vector3d pos2 = rot * pos;
  
  // length of line segment l3 from hip to ankle
  double l3 = pos2.tail<2>().norm();
  double l3sq = l3 * l3;

  // angle of l3 with vertical line
  double gamma = atan2(pos2.y() , -pos2.z());
  
  // angle between l3 and l1
  double alpha = acos((l1sq + l3sq - l2sq) / (2 * l1 * l3));
  
  // angle between l1 and l2
  double beta = acos((l1sq + l2sq - l3sq) / (2 * l1 * l2));
  
  res(Types::LLEG2 - Types::LLEG1) = gamma + alpha + j2offset;
  res(Types::LLEG4 - Types::LLEG1) = beta - M_PI + j4offset;
  
  return res;
}
