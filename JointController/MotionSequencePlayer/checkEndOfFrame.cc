#include "motionsequenceplayer.ih"

void MotionSequencePlayer::checkEndOfFrame()
{
  Clock& clock = SClock::getInstance();
  AgentModel& am = SAgentModel::getInstance();

  // Time at the end of this time step
  double t = (clock.getTime() - d_frameStartTime) + 0.02;
  if (t >= d_currentFrame->time + d_currentFrame->pauseTime)
  {
    d_previousFrame = d_currentFrame;
    d_previousFrame++;
    if (d_currentFrame == d_sequence.end())
      d_currentFrame == d_sequence.begin();
    d_frameStartTime = 0;
  }
}
