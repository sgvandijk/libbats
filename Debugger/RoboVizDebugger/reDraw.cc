#include "robovizdebugger.ih"

void RoboVizDebugger::reDraw()
{
//   if (bats::SLocalizer::initialized())
//   {
//     drawSelf();
//     drawBall();
//     drawPlayers();
//   }
  drawShapes();

  //drawCurve();

  std::string setName = getSetName(std::string());
  swapBuffers(&setName);
}
