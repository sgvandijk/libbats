#include "behavior.ih"

void Behavior::update()
{
 
  Clock& clock = SClock::getInstance();
  
	// Check if the last chosen subbehaviors are committed
	d_childrenCommitted = false;
	for (unsigned i = 0; i < d_lastChosen.size(); ++i)
	{
	  if (d_lastChosen[i]->getLastUpdate() < clock.getTime())
  	  d_lastChosen[i]->update();
  	  
	  if (d_lastChosen[i]->shouldCommit() && d_lastChosen[i]->isCommitted())
	    d_childrenCommitted = true;
	}
  
  d_committed = false;
  if (d_scicc && d_childrenCommitted)
    d_committed = true;
  
  d_lastUpdate = clock.getTime();
}

