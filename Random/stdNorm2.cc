#include "random.ih"

VectorXd Random::stdNorm(unsigned size)
{
  VectorXd vec(size);
  for (unsigned i = 0; i < size; ++i)
    vec(i) = stdNorm();
  return vec;
}
