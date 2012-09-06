#ifndef BATS_JOINTCONTROLLER_HH
#define BATS_JOINTCONTROLLER_HH

#include "../Configurable/configurable.hh"
#include "../Types/types.hh"
#include <string>

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
      : Configurable("jointController", id),
	d_id(id),
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
