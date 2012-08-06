#include "trainersocketcomm.ih"

void TrainerSocketComm::init()
{
  rf<Predicate> msg = makeInitMessage();
  send(msg);
}

rf<Predicate> TrainerSocketComm::makeInitMessage()
{
  rf<Predicate> msg = new Predicate("init");
  return msg;
}
