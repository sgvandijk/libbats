#include "kalmanlocalizer.ih"

void KalmanLocalizer::resetBall()
{
  ball->posVelGlobal->setMu(VectorXd::Zero(6));
  ball->posVelGlobal->setSigma(MatrixXd::Identity(6,6) * 30.0);
}
