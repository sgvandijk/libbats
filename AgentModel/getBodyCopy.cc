#include "agentmodel.ih"

shared_ptr<BodyPart> AgentModel::getBodyCopy()
{
  return getBodyPartCopy(d_torso);
}

