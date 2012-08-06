#include "normaldistribution.ih"

void NormalDistribution::init(VectorXd const& mu, MatrixXd const& sigma)
{
  d_mu = mu;
  d_sigma = sigma;
  //d_invSigma = sigma.inverse();
}
