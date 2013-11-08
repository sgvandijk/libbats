#include "localizer.ih"

void Localizer::update()
{
  Cochlea& cochlea = bats::SCochlea::getInstance();
  d_haveNewVisionData = cochlea.getTimestamp(Cochlea::iVision) == bats::SClock::getInstance().getTime();
  
  //
  // Copy vision data into raw (unfiltered) fields on ObjectInfo instances.
  //
  updateRaw();
  
  //
  // Integrate gyro measurement data into the global rotation matrix and
  // adjust for any drift in the rotation via comparison with vision data.
  //
  updateGlobalRotation();
}
