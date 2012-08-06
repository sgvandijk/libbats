#include "agentmodel.ih"

rf<BodyPart> AgentModel::getBodyCopy()
{
  return getBodyPartCopy(d_torso);
}

