#include "behavior.ih"

ConfidenceInterval Behavior::setGoal(rf<Goal> g)
{
  rf<State> currentState = getCurrentState();
  assert(g);  //without a goal, getting the capability makes no sense
  ConfidenceInterval c = getCapability(currentState, g);
  d_c = c;
  d_goal = g;
  return c;
}
