#include "normaldistribution.ih"

void NormalDistribution::update(shared_ptr<Distribution> obsModel)
{
  // Innovation
  VectorXd y = obsModel->getMu() - d_mu;
  
  // Innovation covariance
  MatrixXd S = d_sigma + obsModel->getSigma();
  
  MatrixXd Sinv = S.inverse();
  
  // Kalman gain
  MatrixXd K = d_sigma * Sinv;
  
  d_mu = d_mu + K * y;
  
  d_sigma = (d_I - K) * d_sigma;
}

