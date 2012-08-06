#include "agentmodel.ih"

bool AgentModel::isDown(bool check) const
{
  Localizer& loc = SLocalizer::getInstance();
    
  if(loc.getGlobalTransformation().matrix().col(2).z() < 0.8)
    return check;
  return !check;
}
