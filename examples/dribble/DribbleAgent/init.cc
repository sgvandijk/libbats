#include "dribbleagent.ih"

void DribbleAgent::init()
{
  // Put initialization stuff here

  // Initialize localizer; use Kalman filter implementation
  SLocalizer::initialize<KalmanLocalizer>();

  // Initialize debugger; use RoboViz implementation
  SDebugger::initialize<RoboVizDebugger>();

  // Initialize gait generator; use Inverse Kinematics implementation
  d_gaitGenerator = make_shared<IKGaitGenerator>("dribblegaitgenerator");
}

