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
    double d_sequenceStartTime;
    Eigen::VectorXd d_startJoingAngles;
    double d_lastRanTime;
    bool d_sequenceDone;

    // Load motion sequence from configuration file
    void createMotionSequence();

    void checkFirstRun();
    void runCurrentStep();
    void checkEndOfSequence();
    
    bool isSequenceDone() const;
  };

  // Member implementations
  inline MotionSequencePlayer::MotionSequencePlayer()
    : JointController("MotionSequencePlayer"),
      d_lastRanTime(0),
      d_sequenceStartTime(0)
  {
    setTag("motionsequenceplayer");
    createMotionSequence();
  }

  inline void MotionSequencePlayer::setSequence(MotionSequence const& sequence)
  {
    d_sequence = sequence;
  }

  inline bool MotionSequencePlayer::isSequenceDone() const
  {
    return d_sequenceDone;
  }
}

#endif
