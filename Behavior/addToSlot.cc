#include "behavior.ih"

void Behavior::addToSlot(shared_ptr<Behavior> behavior, unsigned step, unsigned slot)
{
  shared_ptr<BehaviorNode> behaviorNode = make_shared<BehaviorNode>(behavior);

  if (step >= d_tree->size())
    throw runtime_error("Not enough steps trying to add " + behavior->ident() + " to " + ident());

  if (slot >= d_tree->getChild(step)->size())
    throw runtime_error("Not enough slots trying to add " + behavior->ident() + " to " + ident());

  d_tree->getChild(step)->getChild(slot)->addChild(static_pointer_cast<AST::Node>(behaviorNode));
}
