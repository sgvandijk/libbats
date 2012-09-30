#include "ikgaitgenerator.ih"

IKGaitGenerator::IKGaitGenerator(std::string const& id)
  : GaitGenerator(id)
{
  d_ellipseHeight = getConfParam("/ellipseheight", 0.02);
  d_ellipseLength = getConfParam("/ellipselength", 0.06);
  d_sideWidth = getConfParam("/sidewidth", 0.04);
  d_offsetX = getConfParam("/offsetx", 0.0);
  d_offsetY = getConfParam("/offsety", -0.01);
  d_offsetZ = getConfParam("/offsetz", -0.2);
  d_period = getConfParam("/period", .4);
  d_maxTurnAngle = Math::degToRad(getConfParam("/maxturnangle", 20.0));
  
  d_startupTime = getConfParam("/startuptime", 1.0);

  resetGait();
}
