#include "movejoint.hh"
#include "../../../Action/RC3DAction/rc3daction.hh"
#include "../../../AgentModel/agentmodel.hh"

using namespace bats;
using namespace std;

bool MoveJoint::run()
{
  AgentModel& am = SAgentModel::getInstance();

  shared_ptr<StateVarNode> speedNode = static_pointer_cast<StateVarNode>(d_goal->findDeep("Speed"));

  if (!speedNode)
  {
    return false;
  }
    
  double speed = speedNode->getVar().second.mean();
  am.setControl(d_joint, speed);

  d_action = make_shared<MoveJointAction>(d_joint, speed);
  
  addToActionCommandBehaviors();
  
  return true;
}
