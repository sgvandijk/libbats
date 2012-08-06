#include "trainersocketcomm.ih"

void TrainerSocketComm::setBallPosVel(Vector3d const& pos, Vector3d const& vel)
{
  send(makeSetBallPosVelMessage(pos, vel));
}

rf<Predicate> TrainerSocketComm::makeSetBallPosVelMessage(Vector3d const& pos, Vector3d const& vel)
{
  rf<Predicate> msg = new Predicate("ball");
  rf<Predicate> posPred = rf_cast<Predicate>(msg->addChild(new Predicate("pos")));
  posPred->pushLeafs(pos);
  rf<Predicate> velPred = rf_cast<Predicate>(msg->addChild(new Predicate("vel")));
  velPred->pushLeafs(vel);
  return msg;
}

