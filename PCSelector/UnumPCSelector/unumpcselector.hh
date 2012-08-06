#ifndef __BATS_UNUMPCSELECTOR_HH_
#define __BATS_UNUMPCSELECTOR_HH_

#include "../pcselector.hh"
#include "../../AgentModel/agentmodel.hh"

namespace bats
{
  class UnumPCSelector : public PCSelector
  {
      UnumPCSelector() {}
      UnumPCSelector(UnumPCSelector const&); //NI
      UnumPCSelector operator=(UnumPCSelector const&); //NI
      
    public:
      friend class Singleton<PCSelector>;
      
      virtual unsigned getPlayerClass() { return SAgentModel::getInstance().getUnum(); }
  };
}

#endif
