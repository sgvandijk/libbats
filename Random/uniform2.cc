#include "random.ih"

VectorXd Random::uniform(Eigen::VectorXd const& begin, Eigen::VectorXd const& end)
{
  VectorXd res(begin.size());
  for (int i = 0; i < res.size(); ++i)
    res(i) = uniform(begin(i), end(i));
  return res;
}
