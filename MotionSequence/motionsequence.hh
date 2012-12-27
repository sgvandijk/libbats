#ifndef BATS_MOTIONSEQUENCE_HH
#define BATS_MOTIONSEQUENCE_HH

#include <Eigen/Core>
#include <list>

namespace bats
{

  struct MotionKeyFrame
  {
    Eigen::VectorXd targetAngles;
    double time;
    double pauseTime;
  };
  
  typedef std::list<MotionKeyFrame> MotionSequence;

}

#endif
