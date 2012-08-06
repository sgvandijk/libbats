#include "agentsocketcomm.ih"

void AgentSocketComm::moveTorqueJoint(Types::Joint joint, double force)
{
  send(makeMoveTorqueJointMessage(joint, force));
}
