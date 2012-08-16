#ifndef __BATS_BEHAVIORTREE_HH_
#define __BATS_BEHAVIORTREE_HH_

#include "../Behavior/behavior.hh"
#include <map>

namespace bats
{
  class BehaviorTree
  {
    public:
      typedef std::map<std::string, std::shared_ptr<BehaviorTree> > BTMap;
      
      BehaviorTree(std::string const& id);
      std::shared_ptr<Behavior> getWinBehavior() const { return d_win; }
      
      static void createBehaviorTrees();
      static std::shared_ptr<BehaviorTree> getBehaviorTree(std::string const& id) { return s_behaviorTrees[id]; }
      
    private:
      std::string d_id;
      std::map<std::string, std::shared_ptr<Behavior> > d_behaviors;
      std::shared_ptr<Behavior> d_win;
      
      void create();
      std::shared_ptr<Behavior> create(bats::XMLNode const& b);
      
      static BTMap s_behaviorTrees;
  };
}

#endif
