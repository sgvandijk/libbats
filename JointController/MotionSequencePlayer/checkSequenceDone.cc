#include "motionsequenceplayer.ih"

void MotionSequencePlayer::checkSequenceDone()
{
  Clock& clock = SClock::getInstance();

  double t = clock.getTime() - d_sequenceStartTime;

  d_sequenceDone = t >= d_sequence.length;
}
