#include "ikgaitgenerator.ih"

void IKGaitGenerator::determineSwingFootEnd()
{
  d_curStep.lEnd = d_curStep.rEnd = Vector3d(0, 0, d_offsetZ);

  // stage = 0: swing left
  if (d_stage == 0)
  {
    d_curStep.lEnd.y() = .5 * d_curStep.length + d_offsetY;
    d_curStep.rEnd.y() = -.5 * d_curStep.length + d_offsetY;
    if (d_curStep.width < 0)
    {
      d_curStep.lEnd.x() = .5 * d_curStep.width;
      d_curStep.rEnd.x() = -.5 * d_curStep.width;
    }
  }
  // stage = 1: swing right
  else
  {
    d_curStep.rEnd.y() = .5 * d_curStep.length + d_offsetY;
    d_curStep.lEnd.y() = -.5 * d_curStep.length + d_offsetY;
    if (d_curStep.width > 0)
    {
      d_curStep.lEnd.x() = -.5 * d_curStep.width;
      d_curStep.rEnd.x() = .5 * d_curStep.width;
    }
  }
}
