#include "agentmodel.ih"

void AgentModel::predict()
{
  updatePosture(/*integrateControl*/ true);
}
