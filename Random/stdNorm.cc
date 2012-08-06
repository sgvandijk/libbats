#include "random.ih"

double Random::stdNorm()
{
  double a = uniform();
  double b = uniform();
  
  double x = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
  return x;
}

