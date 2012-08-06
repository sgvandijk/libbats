#include "montecarlodistribution.ih"

void MonteCarloDistribution::setSigma(MatrixXd const& sigma)
{
  // Reinitialize distribution as a normal distribution with given covariance matrix
  init(getMu(), sigma);
}

