#include "helloworldagent.ih"

void HelloWorldAgent::init()
{
  // Put initialization stuff here
  SLocalizer::initialize<KalmanLocalizer>();
  SDebugger::initialize<RoboVizDebugger>();

}

