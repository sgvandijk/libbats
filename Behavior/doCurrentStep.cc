#include "behavior.ih"

#include <algorithm>

bool Behavior::doCurrentStep()
{
  behavior_signal(make_shared<CurrentStepStartEvent>(d_name, d_id, d_curStep));
  
  try
  {
      shared_ptr<AST::Node> curStepNode = d_tree->getChild(d_curStep);

      Clock& clock = SClock::getInstance();
      
      preGenerateGoal(d_curStep);

      if(d_name != "MoveJointTo")
      {
        _debugLevel1(d_id << " capability is: " << getCapability().first << " +- " << getCapability().second);
      }

      
      // Sort all behaviors in the slots by capability
      for (unsigned i = 0; i < curStepNode->size(); ++i)
      {

        shared_ptr<AST::Node> slot = curStepNode->getChild(i);
        if (slot->size() == 0)
          continue;
        
        shared_ptr<Goal> g = generateGoal(d_curStep, i);
        assert(g);
        // Set goals on behaviors in slots
      
        for (unsigned j = 0; j < slot->size(); ++j)
        {
          shared_ptr<Behavior> beh = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
          
      	  if (beh->getLastUpdate() < clock.getTime())
  	        beh->update();
          beh->setGoal(g);
          if(d_id == "play") //debug: print childern of play only
          {
            _debugLevel1("child: \t\t " << beh->getId() << " capability is: " << beh->getCapability().first << " +- " << beh->getCapability().second);
          }
        }
      
        // Sort behaviors by capability
        sort(slot->begin(), slot->end(), CapabilityCompare());
        
        // Give up when the first behavior doesnt have high enough capability
        ConfidenceInterval c = static_pointer_cast<BehaviorNode>(*(slot->begin()))->getBehavior()->getCapability();
        if (c.first + c.second < MIN_C)
        {
          ostringstream out;
          out << ": no behavior is capable in slot " << d_curStep << "-" << i;
          behavior_signal(make_shared<BehaviorEvent>(d_name, d_id, out.str()));
          return false;
        }
      }
      
      bool giveUp = false;
      // Run the behaviors in the slots
      for (unsigned i = 0; i < curStepNode->size() && !giveUp; ++i)
      {
        shared_ptr<AST::Node> slot = curStepNode->getChild(i);
        if (slot->size() == 0)
          continue;
        
        // This is the behavior that ran
        shared_ptr<Behavior> behavior = 0;
        for (unsigned j = 0; j < slot->size() && !behavior; ++j)
        {
          behavior = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
          ConfidenceInterval c = behavior->getCapability();
 
          if (c.first + c.second >= MIN_C && behavior->achieveGoal())
            d_lastChosen.push_back(behavior);
          else
            behavior = 0;
        }
        
        // Reset all behaviors that didn't run
        for (unsigned j = 0; j < slot->size(); ++j)
        {
          shared_ptr<Behavior> b = static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior();
          if (b != behavior)
            b->reset();
        }
        
        // If no behavior ran, give up
        if (!behavior)
        {
          giveUp = true;
        }
      }
       
      // If nothing went wrong, we're in the current step
      if (!giveUp)
      {
        return true;
      }
      // Else reset every behaviour in this step
      else
      {
        behavior_signal(make_shared<BehaviorEvent>(d_name, d_id, ": no behavior ran succesfully"));
        for (unsigned i = 0; i < curStepNode->size(); ++i)
        {
          shared_ptr<AST::Node> slot = curStepNode->getChild(i);
          for (unsigned j = 0; j < slot->size(); ++j)
            static_pointer_cast<BehaviorNode>(slot->getChild(j))->getBehavior()->reset();
        }
        return false;
      }
     
  }
  catch (BatsException *e)
  {
    BATS_CATCH_FRAME(e);
  }
}

