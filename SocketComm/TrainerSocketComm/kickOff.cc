#include "trainersocketcomm.ih"

void TrainerSocketComm::kickOff()
{
  rf<Predicate> msg = makeKickOffMessage();
  send(msg);
}

rf<Predicate> TrainerSocketComm::makeKickOffMessage()
{
  rf<Predicate> msg = new Predicate("kickOff");
  msg->pushLeaf("Left");
  return msg;
}
