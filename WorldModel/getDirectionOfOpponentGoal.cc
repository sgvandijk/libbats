#include "worldmodel.ih"

Vector3d WorldModel::getDirectionOfOpponentGoal() const
{
  Localizer& loc = SLocalizer::getInstance();
  return Math::cartesianToPolar(loc.getTheirGoalMidpointLocal());
}
