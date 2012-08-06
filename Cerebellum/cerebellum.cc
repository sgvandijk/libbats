#include "cerebellum.ih"

Cerebellum::Cerebellum()
{
  // Initialize previous values to absurdity.
  for (unsigned i = 0; i < Types::NJOINTS; ++i)
    d_prevJointValues[i] = -1000.0;
}
