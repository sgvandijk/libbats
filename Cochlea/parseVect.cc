#include "cochlea.ih"

Vector4d Cochlea::parseVect(rf<Predicate> const &pred)
{
  assert(pred);        

  Vector4d res;
  for (unsigned i = 0; i < min(static_cast<unsigned int>(4),pred->size()); ++i)
    res(i) = *rf_cast<Predicate>(pred->getChild(i));
  return res;
}

