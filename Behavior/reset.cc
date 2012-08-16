#include "behavior.ih"

void Behavior::reset()
{
  d_reset = true;
  d_committed = false;
  d_curStepStart = 0;
  
  d_lastChosen.clear();
  
  try
  {
    d_curStep = -1;
    if (!d_tree)
      return;
      
    for (AST::Node::iterator seqIter = d_tree->begin(); seqIter != d_tree->end(); ++seqIter)
    {
      shared_ptr<AST::Node> stepNode = *seqIter;
      for (AST::Node::iterator slotIter = stepNode->begin(); slotIter != stepNode->end(); ++slotIter)
      {
        shared_ptr<AST::Node> slotNode = *slotIter;
        for (AST::Node::iterator behIter = slotNode->begin(); behIter != slotNode->end(); ++behIter)
        {
          shared_ptr<BehaviorNode> bNode = static_pointer_cast<BehaviorNode>(*behIter);
          bNode->getBehavior()->reset();
        }
      }
    }
  }
  catch (BatsException *e)
  {
    BATS_CATCH_FRAME(e);
  }
}
