#include "motionsequenceplayer.ih"

void MotionSequencePlayer::runCurrentStep()
{
  Clock& clock = SClock::getInstance();

  double t = clock.getTime() - d_sequenceStartTime;

  VectorXd targetAngles(Types::NJOINTS);

  //
  // If an end time (duration) was specified
  //
  for (unsigned joint = 0; joint < Types::NJOINTS; ++joint)
  {
    JointKeyFrame thisKf, nextKf;

    // If no key frames for this joint are specified, set angle to zero
    if (d_sequence.jointSequences[joint].size() == 0)
    {
      targetAngles[joint] = 0;
      continue;
    }
    
    // Find the current key frame and the next one
    nextKf = thisKf = d_sequence.jointSequences[joint].front();
    for (JointKeyFrame& kf : d_sequence.jointSequences[joint])
    {
      if (kf.t >= t)
      {
	nextKf = kf;
	break;
      }
      else
	nextKf = thisKf = kf;
    }

    if (thisKf.t == nextKf.t)
      targetAngles[joint] = thisKf.angle;
    else
    {
      // Interpolation ratio
      double alpha = (t - thisKf.t) / (nextKf.t - thisKf.t);

      targetAngles[joint] = (1.0 - alpha) * thisKf.angle + alpha * nextKf.angle;
    }
  }
  
  d_jointVelocities = targetAngles - SAgentModel::getInstance().getJointAngles();
}
