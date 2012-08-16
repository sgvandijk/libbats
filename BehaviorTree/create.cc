#include "behaviortree.ih"

void BehaviorTree::create()
{
  Conf& conf = bats::SConf::getInstance();
  // Find this tree in the configuration
  XMLNodeSet set = conf.selectXPath("/conf/behavior-tree[@id='" + d_id + "']");
  if (set.empty())
    throw runtime_error("(BehaviorTree::create) Behavior tree " + d_id + " does not exist!");
  if (set.size() > 1)
    throw runtime_error("(BehaviorTree::create) Multiple behavior trees with id " + d_id);
  
  // Find the win behavior
  XMLNodeSet winset = conf.selectXPath("/conf/behavior-tree[@id='" + d_id + "']/behaviors/behavior[@id='win']");
  if (winset.empty())
    throw runtime_error("(BehaviorTree::create) Behavior tree " + d_id + " has no win behavior!");
  if (winset.size() > 1)
    throw runtime_error("(BehaviorTree::create) Multiple win behaviors in behavior tree " + d_id);
  
  // Start recursive creation of tree
  d_win = create(winset.front());

  // Iterate over all behaviors, and call 'constructed'
  for (map<string, shared_ptr<Behavior> >::iterator iter = d_behaviors.begin(); iter != d_behaviors.end(); ++iter)
    (iter->second)->constructed();
}

shared_ptr<Behavior> BehaviorTree::create(XMLNode const& b)
{
  Conf& conf = bats::SConf::getInstance();

  // Make behavior of this node's type
  
  BehaviorFactory& bf = bats::SBehaviorFactory::getInstance();
  shared_ptr<Behavior> behavior = bf.createBehavior(b.getProp("type"), b.getProp("id"), d_id);
  
  if (!behavior)
    throw runtime_error(string("(BehaviorTree::create) ") + b.getProp("type") + ": unknown behavior type (trying to create behavior with id: " + b.getProp("id") + " in behavior tree " + d_id + ")");

  behavior->setBehaviorTree(d_id);

  // Go through the slots
  for(XMLNode s = b.getChildren(); s; s = s.getNext())
  {
    if (s.getName() != "slot")
      continue;

    string index = s.getProp("index");
    unsigned step = atoi(index.substr(0, index.find("-")).c_str());
    unsigned slot = atoi(index.substr(index.find("-") + 1).c_str());

    for (XMLNode sb = s.getChildren(); sb; sb = sb.getNext())
      if (sb.getName() == "behavior")
      {

        string path = "/conf/behavior-tree[@id='" + d_id + "']/behaviors/behavior[@id='" + sb.getProp("refid") + "']";
        string commit = sb.getProp("commit");
        string scicc = sb.getProp("scicc");

        XMLNodeSet set = conf.selectXPath(path);
        if (set.size() > 1)
          throw runtime_error(string() + "(BehaviorTree::create) '" + path + "' found multiple times in behavior tree " + d_id + ".");

        if (set.empty())
          throw runtime_error(string() + "(BehaviorTree::create) '" + path + "' not found in behavior tree " + d_id + ".");
        
        XMLNode sb = set.front();
        shared_ptr<Behavior> subBehavior = create(sb);
        subBehavior->shouldCommit(commit == "1");
        subBehavior->shouldCommitIfChildrenCommitted(scicc == "1");
        behavior->addToSlot(subBehavior, step, slot);
      }
  }
  d_behaviors[behavior->getId()] = behavior;

  return behavior;
}

