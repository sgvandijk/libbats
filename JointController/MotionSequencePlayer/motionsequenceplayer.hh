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
    /** Constructor
     *
     * @param id Sequence player id
     */
    MotionSequencePlayer(std::string const& id);
    
    /** Load motion sequence from configuration
     *
     * Your configuration file should have a section of the following format:
     *
     * \code{.xml}
     * <motionsequenceplayer id="playerid">
     *   <sequence>
     *     ...
     *   </sequence>
     *   <issymmetric>1</issymmetric>
     *   </param>
     * </motionsequenceplayer>
     * \endcode
     *
     * The @a issymmetric parameter is optional.
     */
    void loadSequenceFromConf();

    /** Set pre-constructed sequence
     */
    void setSequence(MotionSequence const& sequence);

    /** Run sequence player
     */
    virtual void run(JointControlParams* params);

  private:
    MotionSequence d_sequence;
    double d_sequenceStartTime;
    Eigen::VectorXd d_startJoingAngles;
    double d_lastRanTime;
    bool d_sequenceDone;

    void checkFirstRun();
    void runCurrentStep();
    void checkEndOfSequence();
    
    bool isSequenceDone() const;
  };

  // Member implementations
  inline MotionSequencePlayer::MotionSequencePlayer(std::string const& id)
    : JointController(id),
      d_lastRanTime(0),
      d_sequenceStartTime(0)
  {
    setTag("motionsequenceplayer");
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
