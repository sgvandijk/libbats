#ifndef __BATS_BEHAVIORTREE_HH_
#define __BATS_BEHAVIORTREE_HH_

#include "../Behavior/behavior.hh"
#include <map>

namespace bats
{
  class BehaviorTree : public RefAble
  {
    public:
      typedef std::map<std::string, rf<BehaviorTree> > BTMap;
      
      BehaviorTree(std::string const& id);
      rf<Behavior> getWinBehavior() const { return d_win; }
      
      static void createBehaviorTrees();
      static rf<BehaviorTree> getBehaviorTree(std::string const& id) { return s_behaviorTrees[id]; }
      
    private:
      std::string d_id;
      std::map<std::string, rf<Behavior> > d_behaviors;
      rf<Behavior> d_win;
      
      void create();
      rf<Behavior> create(bats::XMLNode const& b);
      
      static BTMap s_behaviorTrees;
  };
}

#endif
