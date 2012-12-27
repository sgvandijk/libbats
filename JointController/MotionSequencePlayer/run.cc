#include "motionsequenceplayer.ih"

void MotionSequencePlayer::run(JointControlParams* params)
{
  Clock& clock = SClock::getInstance();

  // Did we run last time step?
  checkFirstRun();
  
  // Run the current frame; this sets d_jointVelocities
  runCurrentFrame();
  
  // Check whether we reached the end of this frame, select next if we did
  checkEndOfFrame();
}
