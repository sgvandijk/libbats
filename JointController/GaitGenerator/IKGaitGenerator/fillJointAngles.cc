#include "ikgaitgenerator.ih"

void IKGaitGenerator::fillJointAngles(VectorXd& jointAngles, VectorXd const& leftLegAngles, VectorXd const& rightLegAngles)
{
  AgentModel& am = SAgentModel::getInstance();

  for (int j = Types::LLEG1; j <= Types::LLEG4; ++j)
    jointAngles(j) = leftLegAngles(j - Types::LLEG1);
  for (int j = Types::RLEG1; j <= Types::RLEG4; ++j)
    jointAngles(j) = rightLegAngles(j - Types::RLEG1);

  // Keep feet horizontal
  // Compensate forward/backward
  jointAngles(Types::LLEG5) = -(jointAngles(Types::LLEG2) + jointAngles(Types::LLEG4));
  jointAngles(Types::RLEG5) = -(jointAngles(Types::RLEG2) + jointAngles(Types::RLEG4));

  // Compensate sideways
  jointAngles(Types::LLEG6) = -(jointAngles(Types::LLEG3));
  jointAngles(Types::RLEG6) = -(jointAngles(Types::RLEG3));

  // Compensate turning
  // TODO: optimize, and put into Joint class
  /*
  Vector3d lhipAxis = am.getJoint(Types::LLEG1)->axis;
  Vector3d rhipAxis = am.getJoint(Types::LLEG1)->axis;
  double hipAngle = leftLegAngles(0);

  double lx = lhipAxis.x();
  double ly = lhipAxis.y();
  double lz = lhipAxis.z();
  double rx = rhipAxis.x();
  double ry = rhipAxis.y();
  double rz = rhipAxis.z();

  double sa = sin(hipAngle);
  double ca = cos(hipAngle);

  double lxAngle = atan2(lx * sa, 1.0 - (lx*lx + lz*lz) *(1.0 - ca));
  double lyAngle = atan2(-lx * lz * (1.0 - ca), 1.0 - lz*lz * (1.0 - ca));
  double lzangle = asin(lz * sa);

  double rxAngle = atan2(rx * sa, 1.0 - (rx*rx + rz*rz) *(1.0 - ca));
  double ryAngle = atan2(-rx * rz * (1.0 - ca), 1.0 - rz*rz * (1.0 - ca));
  double rzangle = asin(rz * sa);

  jointAngles(Types::LLEG5) -= lxAngle;
  jointAngles(Types::RLEG5) -= rxAngle;
  jointAngles(Types::LLEG6) -= lyAngle;
  jointAngles(Types::RLEG6) -= ryAngle;
  */

  // Lean
  jointAngles(Types::LLEG2) += d_leanAngle;
  jointAngles(Types::RLEG2) += d_leanAngle;
}
