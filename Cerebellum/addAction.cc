#include "cerebellum.ih"

void Cerebellum::addAction(shared_ptr<Action> action)
{
	d_actions.push_back(action);
}
