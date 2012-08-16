#include "beamto.ih"

shared_ptr<State> BeamTo::getCurrentState()
{
  shared_ptr<State> state = make_shared<State>();
  shared_ptr<OrNode> dis = state->addDisjunct();
  shared_ptr<AndNode> con = dis->addConjunct();

  // con->addVar("Var", 0, 1);
  return state;
}
