#include "ikgaitgenerator.ih"

IKGaitGenerator::IKGaitGenerator(std::string const& id)
  : GaitGenerator(id)
{
  d_ellipseHeight = getConfParam("/ellipseheight", 0.02);
  d_ellipseLength = getConfParam("/ellipselength", 0.03);
  d_sideWidth = getConfParam("/sidewidth", 0.03);
  d_offsetX = getConfParam("/offsetx", 0.0);
  d_offsetY = getConfParam("/offsety", 0.0);
  d_offsetZ = getConfParam("/offsetz", -0.19);
  d_period = getConfParam("/period", 0.5);
  d_maxTurnAngle = Math::degToRad(getConfParam("/maxturnangle", 20.0));
  d_leanAngle = Math::degToRad(getConfParam("/leanangle", 20.0));
  d_startupTime = getConfParam("/startuptime", 2.0);

  resetGait();
}
