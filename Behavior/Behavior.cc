#include "behavior.ih"

Behavior::Behavior(std::string const &name, std::string const &id)
  : d_name(name),
    d_id(id),
    d_tree(0),
    d_curStep(-1),
    d_curStepStart(0),
    d_goal(0),
    d_reset(false),
    d_behaviorTree(""),
    d_c(0,0.2),
    d_action(0),
    d_commit(false),
    d_committed(false),
    d_childrenCommitted(false),
    d_scicc(false),
    d_lastUpdate(0),
    d_lastChosen(),
    d_standard_capability_low(0),
    d_standard_capability_high(0.5)
{
    d_standard_capability_low = getParam("/capability/@low", 0.0);
    d_standard_capability_high = getParam("/capability/@high", 0.5);
}

Behavior::Behavior(std::string const &name, std::string const &id, std::string behaviorTree)
  : d_name(name),
    d_id(id),
    d_tree(0),
    d_curStep(-1),
    d_curStepStart(0),
    d_goal(0),
    d_reset(false),
    d_behaviorTree(behaviorTree),
    d_c(0,0.2),
    d_action(0),
    d_commit(false),
    d_committed(false),
    d_childrenCommitted(false),
    d_scicc(false),
    d_lastUpdate(0),
    d_lastChosen(),
    d_standard_capability_low(0),
    d_standard_capability_high(0.5)
{
    d_standard_capability_low = getParam("/capability/@low", 0.0);
    d_standard_capability_high = getParam("/capability/@high", 0.5);
}
