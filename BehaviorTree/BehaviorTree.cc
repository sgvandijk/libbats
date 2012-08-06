#include "behaviortree.ih"

BehaviorTree::BehaviorTree(std::string const& id)
: d_id(id)
{
  create();
}
