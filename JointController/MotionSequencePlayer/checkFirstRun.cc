#include "motionsequenceplayer.ih"

void MotionSequencePlayer::checkFirstRun() {
  Clock& clock = SClock::getInstance();
  AgentModel& am = SAgentModel::getInstance();

  if (!clock.isPreviousTimeStep(d_lastRanTime))
  {
    // Record starting angles
    d_startJoingAngles = am.getJointAngles();

    // reset start time
    d_sequenceStartTime = clock.getTime();
  }
}
