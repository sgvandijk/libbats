#include "montecarlodistribution.ih"

VectorXd MonteCarloDistribution::draw() const
{
  // Do a roulette wheel selection from the particles
  double ran = Random::uniform(0, d_weights.sum());
  double sum = 0;
  for (int i = 0; i < d_nParticles; ++i)
  {
    sum += d_weights(i);
    if (sum >= ran)
      return d_particles.col(i);
  }
}
