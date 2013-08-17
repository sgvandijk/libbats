/*
 *  Little Green BATS (2008-2013), AI department, University of Groningen
 *
 *  Authors: 	Sander van Dijk (sgdijk@ai.rug.nl)
 *		Drew Noakes (drew@drewnoakes.com)
 *		Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *		Jeroen Kuijpers (jkuypers@gmail.com)
 *
 *  Date: 	August 17, 20013
 *
 *  Website:	https://github.com/sgvandijk/libbats
 *
 *  Comment:	Please feel free to contact us if you have any 
 *		problems or questions about the code.
 *
 *
 *  License: 	This program is free software; you can redistribute 
 *		it and/or modify it under the terms of the GNU General
 *		Public License as published by the Free Software 
 *		Foundation; either version 3 of the License, or (at 
 *		your option) any later version.
 *
 *   		This program is distributed in the hope that it will
 *		be useful, but WITHOUT ANY WARRANTY; without even the
 *		implied warranty of MERCHANTABILITY or FITNESS FOR A
 *		PARTICULAR PURPOSE.  See the GNU General Public
 *		License for more details.
 *
 *   		You should have received a copy of the GNU General
 *		Public License along with this program; if not, write
 *		to the Free Software Foundation, Inc., 59 Temple Place - 
 *		Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef _BATS_AGENTMODEL_HH_
#define _BATS_AGENTMODEL_HH_

#include <vector>
#include <map>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "../BodyPart/bodypart.hh"
#include "../Distribution/NormalDistribution/normaldistribution.hh"
#include "../Singleton/singleton.hh"
#include "../Types/types.hh"
#include "../Math/math.hh"
#include "../Localizer/localizer.hh"
#include "../Clock/clock.hh"

namespace bats
{
  class XMLNode;
  
  /** The AgentModel */
  class AgentModel
  {
  public:
    
    /** Directions the agent could be falling in */
    enum FallDirection
    {
      NOT_FALLING = 0,
      FALL_TO_FRONT,
      FALL_TO_BACK,
      FALL_TO_LEFT,
      FALL_TO_RIGHT
    };
  
    AgentModel();
    virtual ~AgentModel() {}
    
    /**
     * Initialize the agent's body description.
     *
     * This method uses conf.xml to acquire the model data.
     */
    void initBody();

    /** Update the model of the agent. */
    void update();

    /** Update the model by integrating control of last cycle */
    void predict();

    /**
     * Update the posture of a body
     * 
     * @param integrateControl If set to true, the control applied at
     * the previous timestep (ie actions sent to server) are
     * integrated into a prediction of the posture.
     * @param torso The torso (root node) of the body to update. When
     * 0 (default), the AgentModel's internal body will be used.
     * @param updateJoints If set to true, the joint angles in the
     * body will be updated with values from the Cochlea.
     */
    void updatePosture(bool integrateControl = false,
		       std::shared_ptr<BodyPart> torso = 0,
		       bool updateJoints = true);


    /** @returns the name of the Ruby Scene Graph (rsg) file that the simulator uses for this agent. */
    std::string getRSG() const;
    
    /** Set the uniform number of this agent. Only use before initializing the body description. */
    void setUnum(unsigned int const unum);
    
    /** @returns the uniform number of this agent. This number is updated when a number is requested from and given by the simulator. */
    unsigned getUnum() const;
    
    /** @returns the Object representing this agent. This number is updated when a number is requested from and given by the simulator. */
    Types::Object getObject() const;
    
    /**
     * Inverse of getJointName
     * @returns the translation of a joint name into a Types::Joint enumeration value, Types::NJOINTS when joint does not exist.
     */
    Types::Joint getJointID(std::string const& joint) const;
    
    /**
     * Inverse of getJointID
     * @returns the translation of a Types::Joint enumeration value into a string.
     */
    std::string getJointName(Types::Joint const id) const;
    
    /** @returns the information of joint @a joint */
    std::shared_ptr<Joint> getJoint(Types::Joint joint) const;

    /** @returns the position of joint in agent coordinates */
    Eigen::Vector3d getJointPosition(Types::Joint joint) const;
    
    /** @returns a vector containing all current joint angles */
    Eigen::VectorXd getJointAngles();

    /** Notify the agent model of a joint velocity set at this timestep */
    void setControl(Types::Joint joint, double speed);

    /** Notify the agent model of joint velocities set this time step */
    void setControl(Eigen::VectorXd const& control);

    /** @returns the information of body part @a part */
    std::shared_ptr<Limb> getBodyPart(Types::BodyPart part) const;

    Types::BodyPart getLimbID(std::string const& limb) const;

    /** @returns position of body part in agent coordinates */
    Eigen::Vector3d getBodyPartPosition(Types::BodyPart part) const;
    
    /** @returns position of body part in agent coordinates */
    Eigen::Vector3d getBodyPartPositionLocal(Types::BodyPart part) const;

    /** @returns size of body part */
    Eigen::Vector3d getBodyPartSize(Types::BodyPart part) const;
    
    /** @returns the force vector measured by the Force Resistance Perceptor (FRP) of the food on side @a side */
    Eigen::Vector3d getFootForce(Types::Side side) const;
    
    /** @returns the center where the force measured by the Force Resistance Perceptor (FRP) of the food on side @a side acts upon. */
    Eigen::Vector3d getFootForceCenter(Types::Side side) const;
    
    /** @returns the gyroscopic sensor measurement */
    Eigen::Vector3d getGyro() const;
    
    /** @returns the gyroscopic sensor measurement from the previous step */
    Eigen::Vector3d getPreviousGyro() const;
    
    /** @returns the accelerometer sensor measurement */
    Eigen::Vector3d getAcc() const;

    /** @returns the position of the Center Of Mass (COM) of the robot, relative to the center of its torso. */
    Eigen::Vector3d getCOM() const;
    
    /** @returns true when the agent lies on its belly. */
    bool onMyBelly() const;

    /** @returns true when the agent lies on its back. */
    bool onMyBack() const;

    /** @returns true when the agent stands on its feet. */ 
    bool onMyFeet() const;

    /** @returns true when the agent lies on one of its sides. */
    bool onMySide() const;

    /** See whether the agent is down or not
     * @param check Whether you want to check if the agent is down or if he is standing
     */
    bool isDown(bool check = true) const;
    
    /** @returns FallDirection (enum) in which direction the agent is falling */
    FallDirection getDirectionOfFall() { return d_fallDirection; }
    
    std::shared_ptr<BodyPart> getBodyCopy();

    /** @returns The maximum joint velocity, in radians per time step: 351.77 / 180 * pi / 50 */
    double getMaxJointVel() const;

    /** Determine joint angles to achieve position of an ankle
     *
     * This method uses inverse kinematics to determine which angles
     * the joints of a leg should take, in order to move the ankle to
     * a certain position.
     *
     * @param anklePosition Desired position of the ankle, relative to torso
     * @param leg Which leg to determine the angles for
     * @param hipAngle The angle that the hip joint (LEG1) will have
     * @returns a vector of length 6 with the angles for the leg joints
     */
    Eigen::VectorXd jointAnglesForAnklePosition(Eigen::Vector3d const& anklePosition, Types::Side leg, double hipAngle);

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:

    std::string d_rsg;
    unsigned d_unum;
    
    std::shared_ptr<BodyPart> d_torso;
    double d_weight;
    Eigen::Vector3d d_COM;
    
    Eigen::Vector3d d_footForceCenters[2];
    Eigen::Vector3d d_footForces[2];
    
    Eigen::Vector3d d_gyro;
    Eigen::Vector3d d_previousGyro;
    
    Eigen::Vector3d d_acc;
    
    FallDirection d_fallDirection;

    std::map<std::string, Types::Joint> d_jointNameMap;
    std::map<Types::Joint, std::shared_ptr<Joint> > d_joints;
    
    std::map<std::string, Types::BodyPart> d_limbNameMap;
    std::map<Types::BodyPart, std::shared_ptr<Limb> > d_limbs;
    
    double d_tLastControl;
    
    // The modeled maximum kicking distance of the agent.
    std::shared_ptr<NormalDistribution> d_kickMaxDistance;

    // The modeled maximum kicking speed of the agent.
    std::shared_ptr<NormalDistribution> d_kickMaxSpeed;

    /** Recursive method to traverse XML to initialize body model
     *
     * @param node Current body part configuration node
     * @param joint Joint current body part should be connected to
     * @returns Mass accumulated by this body part and recursive calls
     */
    double initBody(XMLNode& node, std::shared_ptr<Joint> joint);
    
    void updateCOM();
    void checkDirectionOfFall();

    std::shared_ptr<BodyPart> getBodyPartCopy(std::shared_ptr<BodyPart> part);
  };

  typedef Singleton<AgentModel> SAgentModel;
  
  /*******************************
   * 
   * Inline method implementations
   * 
   *******************************/
  
  inline std::string AgentModel::getRSG() const { return d_rsg; }
  
  inline unsigned AgentModel::getUnum() const { return d_unum; }
  
  inline Types::Object AgentModel::getObject() const { return Types::getTeamMateWithUnum(d_unum); }
  
  inline Types::Joint AgentModel::getJointID(std::string const& joint) const
  {
    std::map<std::string, Types::Joint>::const_iterator j = d_jointNameMap.find(joint);
    return j == d_jointNameMap.end() ? Types::NJOINTS : j->second;
  }
  
  inline std::shared_ptr<Joint> AgentModel::getJoint(Types::Joint joint) const
  {
    return d_joints.find(joint)->second;
  }

  inline Eigen::VectorXd AgentModel::getJointAngles()
  {
    Eigen::VectorXd jointAngles(Types::NJOINTS);
    for (int j = 0; j < Types::NJOINTS; ++j)
      jointAngles(j) = d_joints[(Types::Joint)j]->angle->getMu()(0);
    return jointAngles;
  }
  
  inline void AgentModel::setControl(Types::Joint joint, double speed)
  {
    d_joints[joint]->control = speed;
    d_tLastControl = SClock::getInstance().getTime();
  }

  inline void AgentModel::setControl(Eigen::VectorXd const& control)
  {
    for (int j = 0; j < Types::NJOINTS; ++j)
      d_joints[(Types::Joint)j]->control = control(j);
    d_tLastControl = SClock::getInstance().getTime();
  }
  
  inline Eigen::Vector3d AgentModel::getJointPosition(Types::Joint joint) const
  {
    return getJoint(joint)->transform.translation();
  }

  inline std::shared_ptr<Limb> AgentModel::getBodyPart(Types::BodyPart part) const
  {
    return d_limbs.find(part)->second;
  }

  inline Types::BodyPart AgentModel::getLimbID(std::string const& limb) const
  {
    std::map<std::string, Types::BodyPart>::const_iterator j = d_limbNameMap.find(limb);
    return j == d_limbNameMap.end() ? Types::NBODYPARTS : j->second;
  }
  

  inline Eigen::Vector3d AgentModel::getBodyPartPosition(Types::BodyPart part) const
  {
    return getBodyPart(part)->transform.translation();
  }
  
  inline Eigen::Vector3d AgentModel::getBodyPartPositionLocal(Types::BodyPart part) const
  {
    return SLocalizer::getInstance().getLocalTransformation() * getBodyPart(part)->transform.translation();
  }
  
  inline Eigen::Vector3d AgentModel::getBodyPartSize(Types::BodyPart part) const
  {
    return getBodyPart(part)->size;
  }

  inline Eigen::Vector3d AgentModel::getFootForce(Types::Side side) const
  {
    return d_footForces[side];
  }

  inline Eigen::Vector3d AgentModel::getFootForceCenter(Types::Side side) const
  {
    return d_footForceCenters[side];
  }

  inline Eigen::Vector3d AgentModel::getGyro() const
  {
    return d_gyro;
  }
  
  inline Eigen::Vector3d AgentModel::getPreviousGyro() const
  {
    return d_previousGyro;
  }

  inline Eigen::Vector3d AgentModel::getAcc() const
  {
    return d_acc;
  }
  
  inline Eigen::Vector3d AgentModel::getCOM() const
  {
    return d_COM;
  }

  inline double AgentModel::getMaxJointVel() const
  {
    return 0.122790894;
  }
}

#endif
