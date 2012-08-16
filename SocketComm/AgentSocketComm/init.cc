#include "agentsocketcomm.ih"

void AgentSocketComm::init(unsigned unum, string team)
{
  send(makeInitMessage(unum, team));
}

shared_ptr<Predicate> AgentSocketComm::makeInitMessage(unsigned unum, string team)
{
  shared_ptr<Predicate> initPred = make_shared<Predicate>("init");
  shared_ptr<Predicate> unumPred = static_pointer_cast<Predicate>(initPred->addChild(make_shared<Predicate>("unum")));
  unumPred->pushLeaf(unum);
  shared_ptr<Predicate> teamPred = static_pointer_cast<Predicate>(initPred->addChild(make_shared<Predicate>("teamname")));
  teamPred->pushLeaf(team);
  return initPred;
}

