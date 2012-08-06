#include "random.ih"

double Random::uniform(double begin, double end)
{
  return 1.0 * rand() / (RAND_MAX + 1.0) * (end - begin) + begin;
}

