#include "montecarlodistribution.ih"

void MonteCarloDistribution::predict(MatrixXd const& F, rf<Distribution> controlModel)
{
  for (unsigned i = 0; i < d_nParticles; ++i)
  {
    // New particle position: x_{t+1} = F * x_t + u, where u is drawn from the control model
    d_particles.col(i) = F * d_particles.col(i) + controlModel->draw();
  }
}
