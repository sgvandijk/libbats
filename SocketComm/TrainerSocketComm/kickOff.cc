#include "trainersocketcomm.ih"

void TrainerSocketComm::kickOff()
{
  shared_ptr<Predicate> msg = makeKickOffMessage();
  send(msg);
}

shared_ptr<Predicate> TrainerSocketComm::makeKickOffMessage()
{
  shared_ptr<Predicate> msg = make_shared<Predicate>("kickOff");
  msg->pushLeaf("Left");
  return msg;
}
