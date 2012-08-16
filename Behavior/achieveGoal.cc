#include "behavior.ih"

bool Behavior::achieveGoal()
{
  behavior_signal(make_shared<AchieveGoalStartEvent>(d_name, d_id));
  
  string msg;
  s_behStack.push_back(shared_from_this());
  
  try {
    if (!d_tree)
    {
      s_behStack.pop_back();
      return true;
    }
    
    bool succes = true;

    // If the last chosen behaviors are committed, run them again
		if (d_childrenCommitted)
		{
      succes = doLastChosen();
      msg = succes ? ": doLastChosen: succes" : ": doLastChosen: fail";
      behavior_signal(make_shared<BehaviorEvent>(d_name, d_id, msg));
    }
    else
    {
      d_lastChosen.clear();
      succes = false;
      
      // Try slots in next sequence step (if there is a step left and we are not committed)
      if (d_curStep >= 0)
      {
        double prevStepStart = d_curStepStart;
        d_curStepStart = SClock::getInstance().getTime();
        succes = doNextStep();
        msg = succes ? ": doNextStep: succes" : ": fail";
        behavior_signal(make_shared<BehaviorEvent>(d_name, d_id, msg));
        if (!succes)
          d_curStepStart = prevStepStart;
      }
      else
      {
        d_curStep = 0;
        d_curStepStart = SClock::getInstance().getTime();
      }
        
      if (!succes)
      {
        succes = doCurrentStep();
        msg = succes ? ": doCurrentStep: succes" : ": fail";
        behavior_signal(make_shared<BehaviorEvent>(d_name, d_id, msg));
      }
    }
    s_behStack.pop_back();
    
    if (succes)
    {
      behavior_signal(make_shared<AchieveGoalSuccessEvent>(d_name, d_id));
      achieveGoal_success_signal(make_shared<AchieveGoalSuccessEvent>(d_name, d_id));
    }
    
    return succes;

  } catch (BatsException *e) {
    s_behStack.pop_back();
    BATS_CATCH_FRAME(e);
  }
  d_reset = false;
  s_behStack.pop_back();
}

