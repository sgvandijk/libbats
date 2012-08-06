#include "predicate.hh"

using namespace bats;
using namespace std;

string Predicate::toString() const
{
  std::ostringstream o;
  //  o.precision(5); //Make sure we don't place too many decimals on the output (doesn't work! (predicates are only strings :)).
  generate(o);
  return o.str();
}
