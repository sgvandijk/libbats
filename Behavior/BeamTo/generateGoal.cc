#include "beamto.ih"

rf<Goal> BeamTo::generateGoal(unsigned step, unsigned slot)
{
  WorldModel& wm = SWorldModel::getInstance();
  
  rf<Goal> goal = new Goal();
  rf<OrNode> dis = goal->addDisjunct();
  rf<AndNode> con = dis->addConjunct();
  
  if (wm.weGetKickOff())
    con->addVar("", d_posWeGetKickOff);
  else
    con->addVar("", d_posTheyGetKickOff);

  return goal;
}
