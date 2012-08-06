#include "beamto.ih"

ConfidenceInterval BeamTo::getCapability(rf<State> s, rf<Goal> g)
{
	WorldModel& wm = SWorldModel::getInstance();
	
  Types::PlayMode pm = wm.getPlayMode();   

  // If we aren't in BEFORE_KICKOFF, our capability is -1.0 with infinite confidence
  if (pm != Types::BEFORE_KICKOFF && pm != Types::GOAL_US && pm != Types::GOAL_THEM)
    return capable(false);
  
  // Else we can certainly do it
  return ConfidenceInterval(1.0, 0);

}
