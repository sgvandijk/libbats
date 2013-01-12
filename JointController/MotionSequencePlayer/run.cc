#include "motionsequenceplayer.ih"

void MotionSequencePlayer::run(JointControlParams* params)
{
  Clock& clock = SClock::getInstance();

  // Did we run last time step? If not, start from beginning
  checkFirstRun();
  
  // Run this timestep
  runCurrentStep();

  // Check whether we are finished
  
}
