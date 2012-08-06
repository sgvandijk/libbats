#include "robovizdebugger.ih"

RoboVizDebugger::RoboVizDebugger()
: d_robovizHost("localhost"),
  d_robovizPort("32769"),
  d_isStarted(false),
  d_isVerbose(false),
  d_sockfd(0),
  d_p(0)
{
}

