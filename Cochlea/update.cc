#include "cochlea.ih"

void Cochlea::update()
{
  rf<Predicate> pred = bats::SAgentSocketComm::getInstance().getPred();
  if (pred)
  {
    integrate(pred);
    synthesizeDerivedData();
  }
}
