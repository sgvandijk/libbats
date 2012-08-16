#include "trainersocketcomm.ih"

void TrainerSocketComm::init()
{
  shared_ptr<Predicate> msg = makeInitMessage();
  send(msg);
}

shared_ptr<Predicate> TrainerSocketComm::makeInitMessage()
{
  shared_ptr<Predicate> msg = make_shared<Predicate>("init");
  return msg;
}
