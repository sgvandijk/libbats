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

  // Initialize walk parameter filter; stationary, facing forward
  d_paramFilter = VectorXd(6);
  d_paramFilter << 0, 0, 0, 0, 1, 0;

  // Create motion sequence players
  d_motionSequencePlayers["getupfromback"]  = make_shared<MotionSequencePlayer>("getupfromback");
  d_motionSequencePlayers["getupfromfront"] = make_shared<MotionSequencePlayer>("getupfromfront");

  // Load the motion sequences from configuration file
  for (auto kv : d_motionSequencePlayers)
    kv.second->loadSequenceFromConf();

  // We have not beamed yet
  d_beamed = false;

}

