#include "behavior.ih"

std::map<std::string, shared_ptr<Behavior> > Behavior::s_behaviors;
std::set<shared_ptr<Behavior> > Behavior::s_actionCommandBehaviors;
shared_ptr<Behavior> Behavior::s_win;
sigc::signal<void, shared_ptr<BehaviorEvent> > Behavior::behavior_signal;
sigc::signal<void, shared_ptr<AchieveGoalSuccessEvent> > Behavior::achieveGoal_success_signal;
std::list<shared_ptr<Behavior> > Behavior::s_behStack;
