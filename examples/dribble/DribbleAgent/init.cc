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

  // Initialize walk paramaeter filter; stationary, facing forward
  d_paramFilter = VectorXd(6);
  d_paramFilter << 0, 0, 0, 0, 1, 0;

}

