#include "batsplayerclass.ih"

BATSPlayerClass::BATSPlayerClass(unsigned idx)
: PlayerClass(idx)
{
  Conf& conf = SConf::getInstance();
  ostringstream idxout;
  idxout << idx;
  
  XMLNodeSet set = conf.selectXPath("/conf/player-class[@index=" + idxout.str() + "]/behavior-tree");
  if (set.empty())
    throw runtime_error("(BATSPlayerClass) Player class " + idxout.str() + " does not have behavior tree!");
    
  XMLNode BTNode = set.front();
  d_behaviorTree = BehaviorTree::getBehaviorTree(BTNode.getProp("refid"));
}
