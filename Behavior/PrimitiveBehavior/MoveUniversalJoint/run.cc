#include "moveuniversaljoint.hh"
#include "../../../Action/RC3DAction/rc3daction.hh"

using namespace bats;
using namespace std;

bool MoveUniversalJoint::run()
{
  shared_ptr<StateVarNode> jointNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Joint"));
  shared_ptr<StateVarNode> speedNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Speed"));

  if (!speedNode || !jointNode)
  {
    return false;
  }
  
  d_lastSpeeds[(unsigned)(jointNode->getVar().second.mean() - d_joint)] = speedNode->getVar().second.mean();
  
  d_action = make_shared<MoveUniversalJointAction>(d_joint, d_lastSpeeds[0], d_lastSpeeds[1]);
  
  addToActionCommandBehaviors();
  
  return true;
}
