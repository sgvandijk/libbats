#include "montecarlodistribution.ih"

MatrixXd MonteCarloDistribution::getSigma() const
{
  // Determine covariance matrix, no idea if this is the best/official way
  VectorXd mu = getMu();
  MatrixXd sigma = MatrixXd::Zero(mu.size(), mu.size());
  for (unsigned i = 0; i < d_nParticles; ++i)
  {
    VectorXd diff = d_particles.col(i) - mu;
    sigma += d_weights(i) * diff * diff.transpose();
  }
  sigma /= d_weights.sum();
  return sigma;
}
