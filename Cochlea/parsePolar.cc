#include "cochlea.ih"

Vector4d Cochlea::parsePolar(rf<Predicate> const &pred)
{
  assert(pred);

  Vector4d vector = parseVect(pred);
  vector.y() = (vector.y() / 180.0 * M_PI);
  vector.z() = (vector.z() / 180.0 * M_PI);
  return vector;
}

