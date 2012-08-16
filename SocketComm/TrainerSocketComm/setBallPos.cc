#include "trainersocketcomm.ih"

void TrainerSocketComm::setBallPos(Vector3d const& pos)
{
  send(makeSetBallPosMessage(pos));
}

shared_ptr<Predicate> TrainerSocketComm::makeSetBallPosMessage(Vector3d const& pos)
{
  shared_ptr<Predicate> msg = make_shared<Predicate>("ball");
  shared_ptr<Predicate> posPred = static_pointer_cast<Predicate>(msg->addChild(make_shared<Predicate>("pos")));
  posPred->pushLeafs(pos);
  return msg;
}

