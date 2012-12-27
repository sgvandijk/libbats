#ifndef BATS_MOTIONSEQUENCEPLAYER_HH
#define BATS_MOTIONSEQUENCEPLAYER_HH

#include "../jointcontroller.hh"
#include "../../MotionSequence/motionsequence.hh"
#include <list>
#include <Eigen/Core>

namespace bats
{

  class MotionSequencePlayer : public JointController
  {
  public:
    MotionSequencePlayer();
    
    void setSequence(MotionSequence const& sequence);

    virtual void run(JointControlParams* params);

  private:
    MotionSequence d_sequence;
    double d_lastRanTime;
    MotionSequence::iterator d_previousFrame;
    MotionSequence::iterator d_currentFrame;
    double d_frameStartTime;

    void createMotionSequence();

    void checkFirstRun();
    void runCurrentFrame();
    void checkEndOfFrame();
  };

  // Member implementations
  inline MotionSequencePlayer::MotionSequencePlayer()
    : JointController("MotionSequencePlayer"),
      d_lastRanTime(0),
      d_frameStartTime(0)
  {
    setTag("motionsequenceplayer");
    createMotionSequence();
  }

  inline void MotionSequencePlayer::setSequence(MotionSequence const& sequence)
  {
    d_sequence = sequence;
  }
}

#endif
