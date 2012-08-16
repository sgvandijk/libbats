#include "cochlea.ih"

Vector4d Cochlea::parseScalar(shared_ptr<Predicate> const &pred)
{
  Vector4d res;
  res.x() = *static_pointer_cast<Predicate>(pred->getChild(0));
  return res;
}

