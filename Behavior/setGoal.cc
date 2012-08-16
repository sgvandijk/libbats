#include "behavior.ih"

ConfidenceInterval Behavior::setGoal(shared_ptr<Goal> g)
{
  shared_ptr<State> currentState = getCurrentState();
  assert(g);  //without a goal, getting the capability makes no sense
  ConfidenceInterval c = getCapability(currentState, g);
  d_c = c;
  d_goal = g;
  return c;
}
