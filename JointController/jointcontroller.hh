#ifndef BATS_JOINTCONTROLLER_HH
#define BATS_JOINTCONTROLLER_HH

#include "../Configurable/configurable.hh"
#include "../Types/types.hh"
#include <string>
#include <Eigen/Core>

namespace bats
{
  /** base class for JointController parameters
   */
  struct JointControlParams
  {
  };

  /** Abstract base class for joint controllers
   */
  class JointController : public Configurable
  {
  public:
    JointController(std::string const& id)
      : Configurable("jointcontroller", id),
	d_jointVelocities(Eigen::VectorXd::Zero(Types::NJOINTS))
    {}

    virtual ~JointController() {}

    virtual void run(JointControlParams* params) = 0;

    Eigen::VectorXd getJointVelocities() const { return d_jointVelocities; }

  protected:
    
    Eigen::VectorXd d_jointVelocities;
  };
}

#endif
