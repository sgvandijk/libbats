#include "kalmanlocalizer.ih"

void KalmanLocalizer::resetBall()
{
  d_ball->posVelGlobal->setMu(VectorXd::Zero(6));
  d_ball->posVelGlobal->setSigma(MatrixXd::Identity(6,6) * 30.0);
}
