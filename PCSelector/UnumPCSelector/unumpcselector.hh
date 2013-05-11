#ifndef BATS_UNUMPCSELECTOR_HH
#define BATS_UNUMPCSELECTOR_HH

#include "../pcselector.hh"
#include "../../AgentModel/agentmodel.hh"

namespace bats
{
  /** Uniform number based player class selector implementation
   *
   * With this implementation, the agent's player class is simply
   * equal to its uniform number.
   */
  class UnumPCSelector : public PCSelector
  {
  public:
    friend class Singleton<PCSelector>;
    
    virtual unsigned getPlayerClass() { return SAgentModel::getInstance().getUnum(); }

  private:
    UnumPCSelector() {}
    UnumPCSelector(UnumPCSelector const&); //NI
    UnumPCSelector operator=(UnumPCSelector const&); //NI
      
  };
}

#endif
