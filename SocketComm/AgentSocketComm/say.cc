#include "agentsocketcomm.ih"

void AgentSocketComm::say(string message)
{
  send(makeSayMessage(message));
}

shared_ptr<Predicate> AgentSocketComm::makeSayMessage(string message)
{
  shared_ptr<Predicate> pred = make_shared<Predicate>("say");
  pred->pushLeaf(message);
  return pred;
}

