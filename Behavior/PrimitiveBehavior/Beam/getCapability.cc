#include "beam.ih"

ConfidenceInterval Beam::getCapability(rf<State> s, rf<Goal> g)
{
  if (!g->findDeep("X"))
  {
    return ConfidenceInterval(-1.0, 0.0);
  }

  return ConfidenceInterval(1.0, 0.0);
}
