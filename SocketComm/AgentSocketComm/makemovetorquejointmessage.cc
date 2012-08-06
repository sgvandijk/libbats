#include "agentsocketcomm.ih"

rf<Predicate> AgentSocketComm::makeMoveTorqueJointMessage(Types::Joint joint, double force)
{
  AgentModel& am = SAgentModel::getInstance();

  rf<Predicate> message = new Predicate("", Predicate::type_list);
  rf<Predicate> pred = message->push(new Predicate(am.getJoint(joint)->effector));
  pred->pushLeaf(force);

  return message;
}
