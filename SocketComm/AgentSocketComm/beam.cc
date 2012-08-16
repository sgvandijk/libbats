#include "agentsocketcomm.ih"

void AgentSocketComm::beam(Vector3d const& pos)
{
  send(makeBeamMessage(pos));
  /*
  shared_ptr<BeamEvent> event = new BeamEvent(pos);
  cout << event->toString() << endl;
  beam_signal(event);
  */
}

shared_ptr<Predicate> AgentSocketComm::makeBeamMessage(Vector3d const& pos)
{
  shared_ptr<Predicate> beamPred = make_shared<Predicate>("beam");
  beamPred->pushLeafs(pos);
  return beamPred;
}

