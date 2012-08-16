#include "trainer.ih"

ConfidenceInterval Trainer::getCapability(shared_ptr<State> s, shared_ptr<Goal> g)
{
  return capable(true);
}
