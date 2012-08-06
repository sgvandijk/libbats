#include "moveuniversaljoint.hh"
#include "../../../Action/RC3DAction/rc3daction.hh"

using namespace bats;

bool MoveUniversalJoint::run()
{
  rf<StateVarNode> jointNode = rf_cast<StateVarNode>(d_goal->findDeep("Joint"));
  rf<StateVarNode> speedNode = rf_cast<StateVarNode>(d_goal->findDeep("Speed"));

  if (!speedNode || !jointNode)
  {
    return false;
  }
  
  d_lastSpeeds[(unsigned)(jointNode->getVar().second.mean() - d_joint)] = speedNode->getVar().second.mean();
  
  d_action = new MoveUniversalJointAction(d_joint, d_lastSpeeds[0], d_lastSpeeds[1]);
  
  addToActionCommandBehaviors();
  
  return true;
}
