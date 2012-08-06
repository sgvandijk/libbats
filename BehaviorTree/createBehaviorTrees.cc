#include "behaviortree.ih"

void BehaviorTree::createBehaviorTrees()
{
  Conf& conf = SConf::getInstance();
  
  XMLNodeSet BTNS = conf.selectXPath("/conf/behavior-tree");
  for (XMLNodeSet::iterator iter = BTNS.begin(); iter != BTNS.end(); ++iter)
  {
    XMLNode BTNode = *iter;
    if (!BTNode.hasProp("id"))
    {
      continue;
    }
    
    std::string id = BTNode.getProp("id");
    s_behaviorTrees[id] = new BehaviorTree(id);
  }
}
