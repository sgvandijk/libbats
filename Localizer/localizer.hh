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
#ifndef BATS_LOCALIZER_HH
#define BATS_LOCALIZER_HH

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include <vector>
#include <functional>

#include "../Cochlea/cochlea.hh"
#include "../Distribution/distribution.hh"
#include "../Distribution/NormalDistribution/normaldistribution.hh"
#include "../Types/types.hh"
#include "../Singleton/singleton.hh"
#include "../WorldModel/worldmodel.hh"
#include "../BatsEvent/batsevent.hh"
#include "../ObjectInfo/objectinfo.hh"
#include "../ObjectInfo/DynamicObjectInfo/dynamicobjectinfo.hh"
#include "../ObjectInfo/DynamicObjectInfo/PlayerInfo/playerinfo.hh"

namespace bats
{
  
  /** The localizer interface

    Localization is done using a class with an interface compatible with this
    one. The specialized version (like SimpleLocalizer) should be placed in the
    Singleton of the Localizer type, which then returns the interface compatible
    specialized class.

    Example: SLocalizer::initialize<SimpleLocalizer>();

    Localizers (and other modules of the library) consider three coordinate
    frames:
    - Agent frame. The origin is at the center of the agent's torso. The
    positive x-axis points to his right (left shoulder to right shoulder), the
    positive y-axis to his front and the positive z-axis upwards (from center of
    torso to head). This is the frame used by AgentModel.
    - Local frame. The origin is at the center of the agent's torso. The
    positive x-axis points to his right, parallel to the field, the positive
    y-axis points to his front, parallel to the field and the positive z-axis
    points upwards, perpendicular to the field. So, this frame is sensitive to
    rotation in the plane parallel to the field, but not to rotation in a
    vertical plane. This frame is most useful for determining positions and
    directions relative to the agent.
    - Global frame. The origin is at the center of the field. The positive
    x-axis points from the origin to the opponent's goal, the positive y-axis to
    the left when facing the opponent's goal and the positive z-axis points
    upward, perpendicular to the field.

    @ingroup localization
  */
  class Localizer
  {
  public:
    typedef std::vector<std::shared_ptr<bats::PlayerInfo> > PlayerVector;
    typedef std::vector<std::shared_ptr<bats::ObjectInfo> > ObjectVector;
    
    Localizer();
    virtual ~Localizer() {}

    ///@name Life cycle
    ///@{

    /** Called to initialize the Localizer */
    virtual void init() = 0;

    /** Called when an update needs to be done */
    virtual void update();

    ///@}

    ///@name Object containers
    ///@{

    /** Gets a vector of all players from both teams. */
    PlayerVector getPlayers() const;

    /** Gets a vector of all opposition players. */
    PlayerVector getOpponents() const;

    /** Gets a vector of all team mates, including the running agent. */
    PlayerVector getTeamMates() const;

    /** Gets a vector of all landmarks, comprising four flags and four goal posts.
     * Note that this only contains goals and flags with objectIds of the form
     * Blah1Us, not Blah1Left. */
    ObjectVector getLandMarks() const;
    
    /** Gets localisation information about the running agent. */
    std::shared_ptr<bats::PlayerInfo> getMe() const;
    
    /** Gets localisation information about the ball. */
    std::shared_ptr<bats::DynamicObjectInfo> getBall() const;
    
    /** Gets a vector of all objects. */
    ObjectVector getObjects() const;
    
    std::shared_ptr<bats::ObjectInfo> getGoal1Us() const;
    std::shared_ptr<bats::ObjectInfo> getGoal2Us() const;
    std::shared_ptr<bats::ObjectInfo> getGoal1Them() const;
    std::shared_ptr<bats::ObjectInfo> getGoal2Them() const;
    
    std::shared_ptr<bats::ObjectInfo> getFlag1Us() const;
    std::shared_ptr<bats::ObjectInfo> getFlag2Us() const;
    std::shared_ptr<bats::ObjectInfo> getFlag1Them() const;
    std::shared_ptr<bats::ObjectInfo> getFlag2Them() const;
    
    std::shared_ptr<bats::ObjectInfo> getFieldCenter() const;

    ///@}

    ///@name Object filters
    ///@{

    /** Get a vector of players, filtered using some predicate
     *
     * @param pred Predicate to filter with. A player is included in
     * the returned list if pred(PlayerInfo) evaluates to true.
     * @param aliveOnly Whether or not to only include players that we have recently seen.
     */
    PlayerVector getFilteredPlayers(std::function<bool(std::shared_ptr<PlayerInfo>)> pred,
                                    bool aliveOnly = true);

    /** Filter a vector of objects, using some predicate
     *
     * @param objects Vector of objects to filer.
     * @param pred Predicate to filter with. An object is included in
     * the returned list if pred(ObjectInfo) evaluates to true.
     */    
    ObjectVector getFilteredObjects(std::vector<std::shared_ptr<ObjectInfo> > objects,
                                    std::function<bool(std::shared_ptr<ObjectInfo>)> pred);

    /** Filter only those objects we know about
     *
     * @param objects Vector of objects to filter.
     * @param pred Optional additional predicate to filter with.
     */
    ObjectVector getAliveObjects(std::vector<std::shared_ptr<DynamicObjectInfo> > objects,
                                 std::function<bool(std::shared_ptr<DynamicObjectInfo>)> pred = 0);

    /** Count the number of objects for which a predicate is true. Deprecated, use std::count instead */
#pragma deprecated
    template <typename ObjectType>
    unsigned count(std::vector<std::shared_ptr<ObjectType>> objects,
                   std::function<bool(std::shared_ptr<ObjectType>)> pred)
    {
      return std::count(objects.begin(), objects.end(), pred);
    };

    /** Get an object based on its ID */
    std::shared_ptr<ObjectInfo> getObjectById(Types::Object object) const { return d_objectInfos[object]; }

    ///@}

    ///@name Transformations, directions and locations
    ///@{

    /** Get the local transformation matrix
     *
     * This matrix describes the transformation from the local frame to the
     * agent frame, i.e. the axes of the agent frame in terms of the local
     * frame. Multiplication of a vector in the agent frame with this matrix
     * results in the position of that vector with respect to the local frame.
     */
    Eigen::Affine3d getLocalTransformation() const;

    /** Get the global transformation matrix
     *
     * This matrix describes the transformation from the global frame to the
     * agent frame, i.e. the axes of the agent frame in terms of the global
     * frame. Multiplication of a vector in the agent frame with this matrix
     * results in the position of that vector with respect to the global
     * frame.
     */
    Eigen::Affine3d getGlobalTransformation() const;

    /** Local to global transform
     *
     * Transform a vector in the local coordinate frame to the globl
     * coordinate frame.
     * @param loc Vector in local frame to transform
     * @param zeroZ Whether to fix the z coordinate in to result to zero
     * @returns a vector in the global frame
     */
    Eigen::Vector3d localToGlobal(Eigen::Vector3d const& loc, bool zeroZ = false);

    /** Global to local transform
     *
     * Transform a point vector in the global coordinate frame to the local
     * coordinate frame.
     * @param glob Vector in global frame to transform
     * @param zeroZ Whether to fix the z coordinate in to result to zero
     * @returns a vector in the local frame
     */
    Eigen::Vector3d globalToLocal(Eigen::Vector3d const& glob, bool zeroZ = false);
    
    /** Local to global rotation
     *
     * Transform a direction vector in the local coordinate frame to
     * the global coordinate frame.
     * @param loc Vector in global frame to transform
     * @returns a vector in the local frame
     */
    Eigen::Vector3d rotateLocalToGlobal(Eigen::Vector3d const& loc);

    /** Global to local rotation
     *
     * Transform a direction vector in the global coordinate frame to
     * the local coordinate frame.
     * @param glob Vector in global frame to transform
     * @returns a vector in the local frame
     */
    Eigen::Vector3d rotateGlobalToLocal(Eigen::Vector3d const& glob);

    /** 
     * @return a unit vector in the local frame's x/y plane that points in
     * the forward direction (towards the opponent's goal, parallel to the
     * sides of the field).
     */
    Eigen::Vector3d getForwardDirLocal() const
    {
      // Rows of transform give global axes in agent frame
      return d_localTransform * d_globalTransform.matrix().block<1,3>(0, 0).transpose();
    }

    /** 
     * @return a unit vector in the local frame's x/y plane that points from
     * the left to the right of the field from the perspective of our goalie
     * looking down the field to the opponent's goal.  This vector is parallel
     * to the goal lines.
     */
    Eigen::Vector3d getRightDirLocal() const
    {
      // Rows of transform give global axes in agent frame
      return d_localTransform * d_globalTransform.matrix().block<1,3>(1, 0).transpose();
    }

    /** @return the midpoint position of the opponent's goal in the local frame. The z-component is zero. */
    #pragma deprecated
    virtual Eigen::Vector3d getTheirGoalMidpointLocal() const { return Eigen::Vector3d(0,1,0); }
    /** @return the midpoint position of our team's goal in the local frame. The z-component is zero. */
    #pragma deprecated
    virtual Eigen::Vector3d getOurGoalMidpointLocal() const  { return Eigen::Vector3d(0,1,0); }
    /** @return the midpoint position of the opponent's goal in the global frame. The z-component is zero. */
    #pragma deprecated
    virtual Eigen::Vector3d getTheirGoalMidpointGlobal() const  { return Eigen::Vector3d(0,1,0); }
    /** @return the midpoint position of our team's goal in the global frame. The z-component is zero. */
    #pragma deprecated
    virtual Eigen::Vector3d getOurGoalMidpointGlobal() const  { return Eigen::Vector3d(0,1,0); }

    ///@}

    ///@name Additional input
    ///@{

    /**
     * Function to tell the localizer that we know our position for sure,
     * for example after we have beamed to somewhere.
     */
    virtual void setGlobalPosition(Eigen::Vector3d &position) { };

    /** Add a new global measurement of an object to integrate */
    virtual void addGlobalMeasurement(std::shared_ptr<DynamicObjectInfo> dynamicObject, 
                                      std::shared_ptr<Distribution> measurement) {};

    /** Notify localizer about a beam event */
    virtual void onBeam(std::shared_ptr<BeamEvent> event) {}

    /** Set camera offset due to calibration error */
    void setCameraOffset(Eigen::Vector3d const& offset)
    {
      d_cameraOffset = offset;
    }

    ///@}

  protected:
    inline Eigen::Vector3d cutPositionVector(Eigen::VectorXd const& posVel) const;
    inline Eigen::Vector3d cutVelocityVector(Eigen::VectorXd const& posVel) const;
    inline Eigen::VectorXd joinPositionAndVelocityVectors(Eigen::Vector3d const& loc, Eigen::Vector3d const& vel) const;
    
    inline Eigen::Matrix3d cutPositionMatrix(Eigen::MatrixXd const& posVel) const;
    inline Eigen::Matrix3d cutVelocityMatrix(Eigen::MatrixXd const& posVel) const;
    inline Eigen::MatrixXd joinPositionAndVelocityMatrices(Eigen::Matrix3d const& loc, Eigen::Matrix3d const& vel) const;
    
    inline void setPositionVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& pos);
    inline void setVelocityVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& vel);

    void updateGlobalRotation();
    
    /** Attempts to estimate the global orientation using vision data.
     * 
     * @param observedGlobalRotation the observed rotation matrix (out param)
     * @returns true if a value could be determined, otherwise false
     */
    bool tryObserveGlobalRotation(Eigen::Matrix3d& observedGlobalRotation);
    
    /** Updates object positions relative to the torso using raw vision data.
     * No filtering occurs.
     */
    void updateRaw();
    
    Cochlea::InfoID getCochleaIdForObject(std::shared_ptr<ObjectInfo> object) const;

    // Whether we have new vision data this timestep
    bool d_haveNewVisionData;

    // Global transformation
    Eigen::Affine3d d_globalTransform;

    // Local transformation
    Eigen::Affine3d d_localTransform;
    
    // The camera offset due to calibration error
    Eigen::Vector3d d_cameraOffset;

    // Array containing all object infos
    std::shared_ptr<ObjectInfo> d_objectInfos[Types::NOBJECTS];

    // Vector holding all the objects
    ObjectVector d_objects;

    // A vector of all players from both teams
    PlayerVector d_players;
    // A vector of all opposition players
    PlayerVector d_opponents;
    // A vector of all team mates, including the running agent
    PlayerVector d_teamMates;

    // A vector of all landmarks, comprising four flags and four goal posts
    ObjectVector d_landmarks;
    
    // Localisation information about the running agent
    std::shared_ptr<bats::PlayerInfo> d_me;
    
    // Localisation information about the ball
    std::shared_ptr<bats::DynamicObjectInfo> d_ball;
        
    std::shared_ptr<bats::ObjectInfo> d_goal1Us;
    std::shared_ptr<bats::ObjectInfo> d_goal2Us;
    std::shared_ptr<bats::ObjectInfo> d_goal1Them;
    std::shared_ptr<bats::ObjectInfo> d_goal2Them;
    
    std::shared_ptr<bats::ObjectInfo> d_flag1Us;
    std::shared_ptr<bats::ObjectInfo> d_flag2Us;
    std::shared_ptr<bats::ObjectInfo> d_flag1Them;
    std::shared_ptr<bats::ObjectInfo> d_flag2Them;
    
    std::shared_ptr<bats::ObjectInfo> d_center;
  };


  // Member implementations

  inline Localizer::ObjectVector Localizer::getObjects() const
  {
    return d_objects;
  }

  inline Localizer::PlayerVector Localizer::getPlayers() const
  {
    return d_players;
  }

  inline Localizer::PlayerVector Localizer::getOpponents() const
  {
    return d_opponents;
  }

  inline Localizer::PlayerVector Localizer::getTeamMates() const
  {
    return d_teamMates;
  }

  inline Localizer::ObjectVector Localizer::getLandMarks() const
  {
    return d_landmarks;
  }
    
  inline std::shared_ptr<bats::PlayerInfo> Localizer::getMe() const
  {
    return d_me;
  }
    
  inline std::shared_ptr<bats::DynamicObjectInfo> Localizer::getBall() const
  {
    return d_ball;
  }
        
  inline std::shared_ptr<bats::ObjectInfo> Localizer::getGoal1Us() const
  {
    return d_goal1Us;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getGoal2Us() const
  {
    return d_goal2Us;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getGoal1Them() const
  {
    return d_goal1Them;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getGoal2Them() const
  {
    return d_goal2Them;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getFlag1Us() const
  {
    return d_flag1Us;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getFlag2Us() const
  {
    return d_flag2Us;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getFlag1Them() const
  {
    return d_flag1Them;
  }

  inline std::shared_ptr<bats::ObjectInfo> Localizer::getFlag2Them() const
  {
    return d_flag2Them;
  }
    
  inline std::shared_ptr<bats::ObjectInfo> Localizer::getFieldCenter() const
  {
    return d_center;
  }

  inline Eigen::Affine3d Localizer::getLocalTransformation() const
  {
    return d_localTransform;
  }

  inline Eigen::Affine3d Localizer::getGlobalTransformation() const
  {
    return d_globalTransform;
  }

  inline Eigen::Vector3d Localizer::localToGlobal(Eigen::Vector3d const& loc, bool zeroZ)
  {
    Eigen::Vector3d global =
      Eigen::Affine3d(getGlobalTransformation() *
                      getLocalTransformation().inverse()) * loc;
    if (zeroZ)
      global.z() = 0;
    return global;
  }

  inline Eigen::Vector3d Localizer::globalToLocal(Eigen::Vector3d const& glob, bool zeroZ)
  {
    Eigen::Vector3d local =
      Eigen::Affine3d(getLocalTransformation() *
                      getGlobalTransformation().inverse()) * glob;
    if (zeroZ)
      local.z() = 0;
    return local;
  }

  inline Eigen::Vector3d Localizer::rotateLocalToGlobal(Eigen::Vector3d const& loc)
  {
    return
      Eigen::Affine3d(getGlobalTransformation().linear() *
                      Eigen::Affine3d(getLocalTransformation().inverse()).linear()) *
      loc;
  }

  inline Eigen::Vector3d Localizer::rotateGlobalToLocal(Eigen::Vector3d const& glob) {
    return
      Eigen::Affine3d(getLocalTransformation().linear() *
                      Eigen::Affine3d(getGlobalTransformation().inverse()).linear()) *
      glob;
  }

  Eigen::Vector3d Localizer::cutPositionVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.head<3>();
  }
  
  Eigen::Vector3d Localizer::cutVelocityVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.tail<3>();
  }

  void Localizer::setPositionVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& pos)
  {
    posVel.head<3>() = pos;
  }
  
  void Localizer::setVelocityVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& vel)
  {
    posVel.tail<3>() = vel;
  }
  
  Eigen::Matrix3d Localizer::cutPositionMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.topLeftCorner<3,3>();
  }

  Eigen::Matrix3d Localizer::cutVelocityMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.bottomRightCorner<3,3>();
  }

  Eigen::VectorXd Localizer::joinPositionAndVelocityVectors(Eigen::Vector3d const& loc, Eigen::Vector3d const& vel) const
  {
    Eigen::VectorXd locVel(6);
    locVel << loc, vel;
    return locVel;
  }

  Eigen::MatrixXd Localizer::joinPositionAndVelocityMatrices(Eigen::Matrix3d const& loc, Eigen::Matrix3d const& vel) const
  {
    Eigen::MatrixXd locVel = Eigen::MatrixXd::Zero(6,6);
    locVel.fill(0);
    locVel.topLeftCorner<3,3>() = loc;
    locVel.bottomRightCorner<3,3>() = vel;
    return locVel;
  }
  
  typedef Singleton<Localizer> SLocalizer;
}

#endif
