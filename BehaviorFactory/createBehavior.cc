#include "behaviorfactory.ih"

shared_ptr<Behavior> BehaviorFactory::createBehavior(string const& key, string const& id, string const& behaviorTree)
{
  if (d_createFunctions.find(key) == d_createFunctions.end())
    throw runtime_error(string() + "createBehavior: no creation function registered for '" + key + "' with id '" + id + "' in behavior tree '" + behaviorTree + "'!");
    
  return (d_createFunctions[key])(id, behaviorTree);
}

