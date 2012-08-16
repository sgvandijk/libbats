#include "trainersocketcomm.ih"

void TrainerSocketComm::setBallPosVel(Vector3d const& pos, Vector3d const& vel)
{
  send(makeSetBallPosVelMessage(pos, vel));
}

shared_ptr<Predicate> TrainerSocketComm::makeSetBallPosVelMessage(Vector3d const& pos, Vector3d const& vel)
{
  shared_ptr<Predicate> msg = make_shared<Predicate>("ball");
  shared_ptr<Predicate> posPred = static_pointer_cast<Predicate>(msg->addChild(make_shared<Predicate>("pos")));
  posPred->pushLeafs(pos);
  shared_ptr<Predicate> velPred = static_pointer_cast<Predicate>(msg->addChild(make_shared<Predicate>("vel")));
  velPred->pushLeafs(vel);
  return msg;
}

