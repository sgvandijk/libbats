#include "cochlea.ih"

void Cochlea::update()
{
  if (bats::SAgentSocketComm::getInstance().hasNextMessage())
  {    
    shared_ptr<Predicate> pred = bats::SAgentSocketComm::getInstance().nextMessage();
    if (pred)
    {
      integrate(pred);
      synthesizeDerivedData();
    }
  }
}
