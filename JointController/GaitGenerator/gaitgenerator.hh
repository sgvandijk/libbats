#ifndef BATS_GAITGENERATOR_HH_
#define BATS_GAITGENERATOR_HH_

#include "../jointcontroller.hh"

namespace bats
{
  /** Enumeration of possible gait types
   * 
   * Each type specifies the purpose of the gait that needs to be generated
   */
  enum GaitType
  {
    GT_WALK,      ///< Walk to a certain point
    GT_DRIBBLE,   ///< Dribble to a certain point
    GT_KICK,      ///< Walk to the ball and kick it to a certain point
    GT_STAND      ///< Stand in place
  };
  
  /** Parameters passed to a gait generator
   */
  struct GaitParams : public JointControlParams
  {
    GaitParams()
      : type(GT_STAND),
	params(1)
    {}

    GaitParams(GaitType t, Eigen::VectorXd p)
    : type(t),
      params(p)
    {}
    
    GaitType type;            ///< Gait type to generate
    Eigen::VectorXd params;   ///< Parameters for gait
  };
  
  /** Abstract class for gait generators
   */
  class GaitGenerator : public JointController
  {
  public:
    GaitGenerator(std::string const& id)
      : JointController(id)
    {
      setTag("gaitgenerator");
    }
  };
  
}

#endif
