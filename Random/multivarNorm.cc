#include "random.ih"

VectorXd Random::multivarNorm(MatrixXd const& covar)
{
  VectorXd vec = stdNorm(covar.cols());
  return covar.llt().matrixL() * vec;
}
