#include "ikgaitgenerator.ih"

void IKGaitGenerator::updateSpeed()
{
  // Get time since last time step
  double dt = SClock::getInstance().getDt();

  // Increase speed such that we go from 0 to full speed in
  // 1 second
  d_speed += dt / d_startupTime;
  d_speed = Util::saturate(d_speed, 0.0, 1.0);
}
