#include "cochlea.ih"
#include "../Debugger/debugger.hh"
#include "../AgentModel/agentmodel.hh"

void Cochlea::synthesizeDerivedData()
{
  if (d_lines.size() == 0)
    return;
  
  auto linesOfExpectedLength = findLines([&](VisibleLine line) { return line.length <= 1.5; });
//   cout << linesOfExpectedLength.size() << " lines of reasonable length" << endl;
  
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
  
//   Debugger& dbg = SDebugger::getInstance();
//   
//   if (dbg.isEnabled() && dbg.isVerbose() && bats::SLocalizer::initialized())
//   {
//     AgentModel& am = bats::SAgentModel::getInstance();
//     Localizer& loc = bats::SLocalizer::getInstance();
// 
//     auto cameraToGlobalTransform = loc.getGlobalTransformation() * am.getBodyPart(Types::HEAD)->transform;
// 
//     for (auto junction : junctions)
//     {
//       Vector3d midpoint = cameraToGlobalTransform * ((junction.line1EndCartesian + junction.line2EndCartesian)/2);
//       dbg.draw(new Point(midpoint, 4, Vector4d(1,0.5,0,1), "Lines.CircleJunctions"));
//     }
//   }
  
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
