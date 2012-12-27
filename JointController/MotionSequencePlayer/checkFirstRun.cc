#include "motionsequenceplayer.ih"

void MotionSequencePlayer::checkFirstRun() {
  Clock& clock = SClock::getInstance();

  if (!clock.isPreviousTimeStep(d_lastRanTime))
  {
    // Push current angles to beginning of list
    MotionKeyFrame startFrame;
    startFrame.targetAngles = SAgentModel::getInstance().getJointAngles();
    d_sequence.push_front(startFrame);
    d_previousFrame = d_sequence.begin();
    d_currentFrame = d_previousFrame;
    d_currentFrame++;
    d_frameStartTime = 0;
  }
}
