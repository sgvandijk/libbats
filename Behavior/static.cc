#include "behavior.ih"

std::map<std::string, rf<Behavior> > Behavior::s_behaviors;
std::set<rf<Behavior> > Behavior::s_actionCommandBehaviors;
rf<Behavior> Behavior::s_win;
sigc::signal<void, rf<BehaviorEvent> > Behavior::behavior_signal;
sigc::signal<void, rf<AchieveGoalSuccessEvent> > Behavior::achieveGoal_success_signal;
std::list<rf<Behavior> > Behavior::s_behStack;
