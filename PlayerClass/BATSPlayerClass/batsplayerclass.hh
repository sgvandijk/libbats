
#ifndef __BATS_BATSPLAYERCLASS_HH_
#define __BATS_BATSPLAYERCLASS_HH_

#include "../playerclass.hh"
#include "../../Behavior/behavior.hh"
#include "../../BehaviorTree/behaviortree.hh"

namespace bats
{
  class BATSPlayerClass : public PlayerClass
  {
  public:
    BATSPlayerClass(unsigned idx);
    std::shared_ptr<BehaviorTree> getBehaviorTree() const { return d_behaviorTree; }
    
  protected:
    std::shared_ptr<BehaviorTree> d_behaviorTree;
  };
    
}

#endif
