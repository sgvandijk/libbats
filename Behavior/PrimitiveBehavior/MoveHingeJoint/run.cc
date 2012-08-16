#include "movehingejoint.hh"
#include "../../../Action/RC3DAction/rc3daction.hh"

using namespace bats;
using namespace std;

bool MoveHingeJoint::run()
{
  shared_ptr<StateVarNode> speedNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Speed"));

  if (!speedNode)
  {
    return false;
  }
    
  double speed = speedNode->getVar().second.mean();
  d_action = make_shared<MoveHingeJointAction>(d_joint, speed);
  
  addToActionCommandBehaviors();
  
  return true;
}
