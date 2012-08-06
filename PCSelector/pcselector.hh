#ifndef __BATS_PCSELECTOR_HH_
#define __BATS_PCSELECTOR_HH_

#include "../Singleton/singleton.hh"

namespace bats
{
  class PCSelector
  {
  public:
    virtual ~PCSelector() {}

    virtual unsigned getPlayerClass() = 0;
    
    virtual void setPlayerClass(unsigned c) {}
  };

  typedef Singleton<PCSelector> SPCSelector;
}

#endif
