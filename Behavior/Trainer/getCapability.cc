#include "trainer.ih"

ConfidenceInterval Trainer::getCapability(rf<State> s, rf<Goal> g)
{
  return capable(true);
}
