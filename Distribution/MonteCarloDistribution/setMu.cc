#include "montecarlodistribution.ih"

void MonteCarloDistribution::setMu(VectorXd const& mu)
{
  // MOve all particles such that the mean is shifted to mu
  VectorXd mu0 = getMu();
  VectorXd diff = mu - mu0;
  for (unsigned i = 0; i < d_nParticles; ++i)
    d_particles.col(i) += diff;
}
