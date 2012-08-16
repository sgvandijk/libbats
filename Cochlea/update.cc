#include "cochlea.ih"

void Cochlea::update()
{
  shared_ptr<Predicate> pred = bats::SAgentSocketComm::getInstance().getPred();
  if (pred)
  {
    integrate(pred);
    synthesizeDerivedData();
  }
}
