#include "beamto.ih"

shared_ptr<Goal> BeamTo::generateGoal(unsigned step, unsigned slot)
{
  WorldModel& wm = SWorldModel::getInstance();
  
  shared_ptr<Goal> goal = make_shared<Goal>();
  shared_ptr<OrNode> dis = goal->addDisjunct();
  shared_ptr<AndNode> con = dis->addConjunct();
  
  if (wm.weGetKickOff())
    con->addVar("", d_posWeGetKickOff);
  else
    con->addVar("", d_posTheyGetKickOff);

  return goal;
}
