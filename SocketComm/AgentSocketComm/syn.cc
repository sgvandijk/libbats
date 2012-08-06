#include "agentsocketcomm.ih"

void AgentSocketComm::syn()
{
  send(makeSynMessage());
}

rf<Predicate> AgentSocketComm::makeSynMessage()
{
  rf<Predicate> syncPred = new Predicate("syn");
  return syncPred;
}

