#include "agentsocketcomm.ih"
shared_ptr<Predicate> AgentSocketComm::makeMoveHingeJointMessage(Types::Joint joint, double deltaAngle)
{
  AgentModel& am = SAgentModel::getInstance();

  deltaAngle = deltaAngle / M_PI * 180.0;
  
  shared_ptr<Predicate> message = make_shared<Predicate>("", Predicate::type_list);
  shared_ptr<Predicate> pred = message->push(make_shared<Predicate>(am.getJoint(joint)->effector));
  pred->pushLeaf(deltaAngle);

  return message;  
}
