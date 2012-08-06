#include "normaldistribution.ih"

double NormalDistribution::getP(const Eigen::VectorXd& x) const
{
  VectorXd d = x - d_mu;
  return 1.0 / pow(2 * M_PI,d_size / 2) * sqrt(d_sigma.determinant()) * exp(-.5 * (d.transpose() * d_invSigma * d)(0,0));
}
