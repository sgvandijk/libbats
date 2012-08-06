#include "agentsocketcomm.ih"

void AgentSocketComm::beam(Vector3d const& pos)
{
  send(makeBeamMessage(pos));
  /*
  rf<BeamEvent> event = new BeamEvent(pos);
  cout << event->toString() << endl;
  beam_signal(event);
  */
}

rf<Predicate> AgentSocketComm::makeBeamMessage(Vector3d const& pos)
{
  rf<Predicate> beamPred = new Predicate("beam");
  beamPred->pushLeafs(pos);
  return beamPred;
}

