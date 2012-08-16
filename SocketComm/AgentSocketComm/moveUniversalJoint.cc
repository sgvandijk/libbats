#include "agentsocketcomm.ih"

void AgentSocketComm::moveUniversalJoint(Types::Joint joint, double deltaAngle1, double deltaAngle2)
{
  send(makeMoveUniversalJointMessage(joint, deltaAngle1, deltaAngle2));
}

shared_ptr<Predicate> AgentSocketComm::makeMoveUniversalJointMessage(Types::Joint joint, double deltaAngle1, double deltaAngle2)
{
  AgentModel& am = SAgentModel::getInstance();
  
  deltaAngle1 = deltaAngle1 / M_PI * 180.0;
  deltaAngle2 = deltaAngle2 / M_PI * 180.0;

  shared_ptr<Predicate> message = make_shared<Predicate>("", Predicate::type_list);
  shared_ptr<Predicate>  pred = message->push(make_shared<Predicate>(am.getJoint(joint)->effector));
  pred->pushLeaf(deltaAngle1);
  pred->pushLeaf(deltaAngle2);
  
  return message;
}
