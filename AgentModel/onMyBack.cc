#include "agentmodel.ih"

bool AgentModel::onMyBack() const
{
  Localizer& loc = SLocalizer::getInstance();

  Affine3d trans = loc.getGlobalTransformation();

  return Math::getForward(trans).z() > 0.90;
}

