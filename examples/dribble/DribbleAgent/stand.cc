#include "dribbleagent.ih"

VectorXd DribbleAgent::stand()
{
  AgentModel& am = SAgentModel::getInstance();
  
  // Target angles: arms down and slightly squatted
  VectorXd targetJointAngles = VectorXd::Zero(Types::NJOINTS);
  targetJointAngles(Types::LARM1) = targetJointAngles(Types::RARM1) = -.5 * M_PI;

  targetJointAngles(Types::LLEG2) = targetJointAngles(Types::RLEG2) = Math::degToRad(35.0);
  targetJointAngles(Types::LLEG4) = targetJointAngles(Types::RLEG4) = Math::degToRad(-50.0);
  targetJointAngles(Types::LLEG5) = targetJointAngles(Types::RLEG5) = Math::degToRad(25.0);

  return 0.05 * (targetJointAngles - am.getJointAngles());

}
