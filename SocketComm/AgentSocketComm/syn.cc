#include "agentsocketcomm.ih"

void AgentSocketComm::syn()
{
  send(makeSynMessage());
}

shared_ptr<Predicate> AgentSocketComm::makeSynMessage()
{
  shared_ptr<Predicate> syncPred = make_shared<Predicate>("syn");
  return syncPred;
}

