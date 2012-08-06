#include "math.ih"

#define DEBUGLEVEL 0
#include "../Preprocessor/debug.hh"


bool Math::atSameSideOf(Vector3d const& pivot, Vector3d const& target, Vector3d const& me)
{
  Vector3d dif = target - pivot;
  _debugLevel1("dif: " << dif);
  
  Vector3d perpendicular = Math::calcPerpend(dif);
  _debugLevel1("perpendicular: " << perpendicular);
  
  double a = perpendicular.y();
  double b = pivot.y() - a * pivot.x();
  
  double yTarget = a * target.x() + b;
  double yMe = a * me.x() + b;
  
  double dyTarget = yTarget - target.y();
  double dyMe = yMe - me.y();
  if (dyTarget * dyMe >= 0)
  {
    _debugLevel1("Yes");
  //if (sign(dyTarget) == sign(dyMe))
    return true;
  }

  _debugLevel1("Yes");
  return false;
}

