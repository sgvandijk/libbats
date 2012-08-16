#include "montecarlodistribution.ih"

void MonteCarloDistribution::update(shared_ptr<Distribution> obsModel)
{
  // Update the weight of the particles according to the probability of it being correct given the observation model
  // w = p(x_{t+1}|obs)
  for (unsigned i = 0; i < d_nParticles; ++i)
  {
    d_weights(i) = obsModel->getP(d_particles.col(i));
  }
}
