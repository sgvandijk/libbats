#include "robovizdebugger.ih"

void RoboVizDebugger::reDraw()
{
  drawShapes();

  //drawCurve();

  std::string setName = getSetName(std::string());
  swapBuffers(&setName);
}
