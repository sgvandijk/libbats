#include "normaldistribution.ih"

void NormalDistribution::predict(MatrixXd const& F, rf<Distribution> controlModel)
{
  // x_k^- = F x_k-1 + B u_k-1
  // where u is the movement model
  // the movement model is 
  // All movement is accumulated in the movement model. so there is no extra movement: A = I and no translation is needed: B = 1, so:
  // x_k^- = x_k-1 + u_k-1
  d_mu = F * d_mu + controlModel->getMu();
  
  // S_k^- = F P_k-1^+ F^T + Q_k-1 = P_k-1^+ + Q_k-1
  d_sigma = F * d_sigma * F.transpose() + controlModel->getSigma();
  
}

