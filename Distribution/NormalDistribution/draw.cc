#include "normaldistribution.ih"

VectorXd NormalDistribution::draw() const
{
  VectorXd z(d_size);
  for (std::size_t i = 0; i < d_size; ++i)
    z(i) = Random::stdNorm();

  return d_mu + d_sigma.llt().matrixL() * z;
}

