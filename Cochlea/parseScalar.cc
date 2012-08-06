#include "cochlea.ih"

Vector4d Cochlea::parseScalar(rf<Predicate> const &pred)
{
  Vector4d res;
  res.x() = *rf_cast<Predicate>(pred->getChild(0));
  return res;
}

