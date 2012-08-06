#include "behavior.ih"

bool Behavior::doLastChosen()
{
  behavior_signal(new LastChosenStartEvent(d_name, d_id));
  
  try
  {
    bool succes = true;
    rf<AST::Node> curStepNode = d_tree->getChild(d_curStep);

    Clock& clock = SClock::getInstance();
    
    // Sort all behaviors in the slots by capability
    preGenerateGoal(d_curStep);
    for (unsigned i = 0; i < curStepNode->size(); ++i)
    {

      rf<AST::Node> slot = curStepNode->getChild(i);
      if (slot->size() == 0)
        continue;
      
      rf<Goal> g = generateGoal(d_curStep, i);
      for (unsigned j = 0; j < slot->size(); ++j)
      {
        rf<Behavior> behavior = rf_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
        if (find(d_lastChosen.begin(), d_lastChosen.end(), behavior) != d_lastChosen.end())
        {
      	  if (behavior->getLastUpdate() < clock.getTime())
  	        behavior->update();
          behavior->setGoal(g);
          succes = succes && behavior->achieveGoal();
        }
      }
    }
    return succes;
    
  }
  catch (BatsException *e)
  {
    BATS_CATCH_FRAME(e);
  }
}
 
