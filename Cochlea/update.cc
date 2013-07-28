#include "cochlea.ih"

void Cochlea::update()
{
  while (bats::SAgentSocketComm::getInstance().hasNextMessage())
  {    
    shared_ptr<Predicate> pred = bats::SAgentSocketComm::getInstance().nextMessage();
    if (pred)
    {
      integrate(pred);
      synthesizeDerivedData();
    }
  }
}
