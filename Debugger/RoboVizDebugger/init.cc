#include "robovizdebugger.ih"

void RoboVizDebugger::init(int argc, char** argv)
{
  for (int a = 1; a < argc; a++)
  {
    if (string(argv[a]) == "-vh")
      {assert(a+1 < argc); d_robovizHost = argv[++a];}
    else if(string(argv[a]) == "-vp")
      {assert(a+1 < argc); d_robovizPort = argv[++a];}
  }
}

