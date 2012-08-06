#include "behavior.ih"

void Behavior::addToSlot(rf<Behavior> behavior, unsigned step, unsigned slot)
{
  rf<BehaviorNode> behaviorNode = new BehaviorNode(behavior);

  if (step >= d_tree->size())
    throw runtime_error("Not enough steps trying to add " + behavior->ident() + " to " + ident());

  if (slot >= d_tree->getChild(step)->size())
    throw runtime_error("Not enough slots trying to add " + behavior->ident() + " to " + ident());

  d_tree->getChild(step)->getChild(slot)->addChild(rf_cast<AST::Node>(behaviorNode));
}
