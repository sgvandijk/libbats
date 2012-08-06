#include "montecarlodistribution.ih"

void MonteCarloDistribution::init(VectorXd const& mu, MatrixXd const& sigma)
{
  NormalDistribution nd(d_size);
  nd.init(mu, sigma);
  
  for (unsigned i = 0; i < d_nParticles; ++i)
  {
    d_particles.col(i) = nd.draw();
    d_weights(i) = nd.getP(d_particles.col(i));
  }
  
}
