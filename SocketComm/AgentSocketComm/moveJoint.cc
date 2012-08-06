#include "agentsocketcomm.ih"

void AgentSocketComm::moveJoint(Types::Joint joint, double deltaAngle)
{
  send(makeMoveJointMessage(joint, deltaAngle));
}

rf<Predicate> AgentSocketComm::makeMoveJointMessage(Types::Joint joint, double deltaAngle)
{
  AgentModel& am = SAgentModel::getInstance();

  rf<Predicate> msg;
  
  rf<Joint> j = am.getJoint(joint);

  switch (j->type) {

  case Types::UNIVERSAL_JOINT:
    if (j->ujindex == 0)
      msg = makeMoveUniversalJointMessage(joint, deltaAngle, 0);
    else
      msg = makeMoveUniversalJointMessage(joint, 0, deltaAngle);
	break;

  case Types::HINGE_JOINT:
    msg = makeMoveHingeJointMessage(joint, deltaAngle);
	break;

  case Types::TORQUE_JOINT:
    msg = makeMoveTorqueJointMessage(joint, deltaAngle);
	break;

  default:
  break;
  
  }

  return msg;
}
