#include "behavior.ih"

bool Behavior::doNextStep()
{
  behavior_signal(make_shared<NextStepStartEvent>(d_name, d_id, d_curStep + 1));
  
  try
  {
    Clock& clock = SClock::getInstance();
    
    if (d_curStep < static_cast<int>(d_tree->size()) - 1)
    {
      shared_ptr<AST::Node> nextStepNode = d_tree->getChild(d_curStep + 1);

      bool stepHasBehaviors = false;      
      // Sort all behaviors in the slots by capability
      preGenerateGoal(d_curStep + 1);
      for (unsigned i = 0; i < nextStepNode->size(); ++i)
      {

        shared_ptr<AST::Node> slot = nextStepNode->getChild(i);
        if (slot->size() == 0)
          continue;
        
        stepHasBehaviors = true;
        
        shared_ptr<Goal> g = generateGoal(d_curStep + 1, i);
        // Set goals on behaviors in slots
      
        for (unsigned j = 0; j < slot->size(); ++j)
        {
          shared_ptr<Behavior> beh = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
      	  if (beh->getLastUpdate() < clock.getTime())
  	        beh->update();
          beh->setGoal(g);
	      }
	      
        // Sort behaviors by capability
        sort(slot->begin(), slot->end(), CapabilityCompare());
      
        // Give up when the first behavior doesnt have high enough capability
        ConfidenceInterval c = static_pointer_cast<BehaviorNode>(*(slot->begin()))->getBehavior()->getCapability();
        if (c.first + c.second < MIN_C)
          return false;
      }
      
      if (!stepHasBehaviors)
        return false;
      
      bool giveUp = false;
      // Run the behaviors in the slots
      for (unsigned i = 0; i < nextStepNode->size() && !giveUp; ++i)
      {
        shared_ptr<AST::Node> slot = nextStepNode->getChild(i);
        if (slot->size() == 0)
          continue;
          
        shared_ptr<Behavior> behavior = 0;
        for (unsigned j = 0; j < slot->size(); ++j)
        {
          behavior = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
          ConfidenceInterval c = behavior->getCapability();
          if (c.first + c.second >= MIN_C && behavior->achieveGoal())
          {
            d_lastChosen.push_back(behavior);
            break;
          }
          else
          {
            behavior = 0;
          }
        }
        
        // Reset all behaviors that didn't run
        for (unsigned j = 0; j < slot->size(); ++j)
        {
          shared_ptr<Behavior> b = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
          if (b != behavior)
            b->reset();
        }
        
        if (!behavior)
        {
          giveUp = true;
        }
      }
       
      // If nothing went wrong, we're in the next step
      if (!giveUp)
      {
        d_curStep++;
        return true;
      }
      // Else reset every behaviour in this step
      else
      {
        for (unsigned i = 0; i < nextStepNode->size(); ++i)
        {
          shared_ptr<AST::Node> slot = nextStepNode->getChild(i);
          for (unsigned j = 0; j < slot->size(); ++j)
            static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior()->reset();
        }
        return false;
      }
    }
    // There is no next step
    else
      return false;
      
  }
  catch (BatsException *e)
  {
    BATS_CATCH_FRAME(e);
  }
}
