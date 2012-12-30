#ifndef BATS_MOTIONSEQUENCE_HH
#define BATS_MOTIONSEQUENCE_HH

#include <Eigen/Core>
#include <list>

namespace bats
{
  
  /// Joint position description in motion sequence
  struct JointKeyFrame
  {
    double t;     ///< Time of frame
    double angle; ///< Joint angle
  };

  /// A joint sequence is a list of keyframes
  typedef std::list<JointKeyFrame> JointSequence;

  /// A motion sequence is a vector of joint sequences;
  struct MotionSequence
  {
    std::vector<JointSequence> jointSequences;
    double length;
  };

}

#endif
