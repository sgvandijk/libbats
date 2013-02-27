#include "cochlea.ih"

void Cochlea::synthesizeDerivedData()
{
  if (d_lines.size() == 0)
    return;
  
  auto linesOfExpectedLength = findLines([&](VisibleLine line) { return line.length <= 1.5; });
  
  std::vector<VisibleLineJunction> junctions;
  for (VisibleLineJunction junction : getLineJunctions(linesOfExpectedLength))
  {
    //
    // Only accept lines that intersect at a junction where the angle between them is 180/5 +/- 10 degrees
    //
    double angle = Math::getAngle(junction.line1.toVector3d(), junction.line2.toVector3d());
    if (Math::normalizeRadPositive(angle) - M_PI/5 < Math::degToRad(10))
      junctions.push_back(junction);
  }
  
  if (junctions.size() == 10)
  {
    Vector3d sum(0,0,0);
    for (VisibleLineJunction junction : junctions)
    {
      sum += junction.line1EndCartesian;
      sum += junction.line2EndCartesian;
    }
    Vector3d midpoint = sum / 20;
    Vector3d midpointPolar = Math::cartesianToPolar(midpoint);
    
    d_info[Cochlea::iVisionFieldCenter] = Vector4d(midpointPolar[0], midpointPolar[1], midpointPolar[2], 0);
    
    auto now = d_info[Cochlea::iNow][0];
    d_dt[Cochlea::iVisionFieldCenter] = now - d_timestamps[Cochlea::iVisionFieldCenter];
    d_timestamps[Cochlea::iVisionFieldCenter] = now;
  }
}
