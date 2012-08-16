#include "trainersocketcomm.ih"

void TrainerSocketComm::setBallVel(Vector3d const& vel)
{
  send(makeSetBallVelMessage(vel));
}

shared_ptr<Predicate> TrainerSocketComm::makeSetBallVelMessage(Vector3d const& vel)
{
  shared_ptr<Predicate> msg = make_shared<Predicate>("ball");
  shared_ptr<Predicate> velPred = static_pointer_cast<Predicate>(msg->addChild(make_shared<Predicate>("vel")));
  velPred->pushLeafs(vel);
  return msg;
}

