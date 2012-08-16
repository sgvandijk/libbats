#include "agentsocketcomm.ih"

shared_ptr<Predicate> AgentSocketComm::makeMoveTorqueJointMessage(Types::Joint joint, double force)
{
  AgentModel& am = SAgentModel::getInstance();

  shared_ptr<Predicate> message = make_shared<Predicate>("", Predicate::type_list);
  shared_ptr<Predicate> pred = message->push(make_shared<Predicate>(am.getJoint(joint)->effector));
  pred->pushLeaf(force);

  return message;
}
