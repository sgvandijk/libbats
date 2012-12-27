#include "motionsequenceplayer.ih"

void MotionSequencePlayer::runCurrentFrame()
{
  Clock& clock = SClock::getInstance();
  AgentModel& am = SAgentModel::getInstance();

  // See if we just started this frame
  if (d_frameStartTime == 0)
    d_frameStartTime = clock.getTime();

  VectorXd currentAngles = am.getJointAngles();

  // Time at the end of this time step
  double t = (clock.getTime() - d_frameStartTime) + 0.02;
  double alpha = t / d_currentFrame->time;
  if (alpha > 1.0)
    alpha = 1;

  VectorXd targetAngles = alpha * (d_currentFrame->targetAngles - d_previousFrame->targetAngles);

  d_jointVelocities = targetAngles - currentAngles;
}
