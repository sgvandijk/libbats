#ifndef BATS_GAITGENERATOR_HH
#define BATS_GAITGENERATOR_HH

#include "../jointcontroller.hh"

namespace bats
{
  
  /** Parameters passed to a gait generator
   */
  struct GaitParams : public JointControlParams
  {
    GaitParams()
      : params(1)
    {}

    GaitParams(Eigen::VectorXd p)
    : params(p)
    {}
    
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
