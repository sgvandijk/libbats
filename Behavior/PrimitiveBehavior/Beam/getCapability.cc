#include "beam.ih"

ConfidenceInterval Beam::getCapability(shared_ptr<State> s, shared_ptr<Goal> g)
{
  if (!g->findDeep("X"))
  {
    return ConfidenceInterval(-1.0, 0.0);
  }

  return ConfidenceInterval(1.0, 0.0);
}
