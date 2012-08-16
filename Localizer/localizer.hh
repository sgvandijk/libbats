/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors: 	Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		Sander van Dijk (sgdijk@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *
 *	All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date: 	November 1, 2008
 *
 *  Website:	http://www.littlegreenbats.nl
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

#ifndef _BATS_LOCALIZER_HH_
#define _BATS_LOCALIZER_HH_

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include <vector>
#include <functional>

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
  */
  class Localizer
  {
  public:
    typedef std::vector<std::shared_ptr<bats::PlayerInfo> > PlayerVector;
    typedef std::vector<std::shared_ptr<bats::ObjectInfo> > ObjectVector;
    
    virtual ~Localizer() {}

    /** Called to initialize the Localizer */
    virtual void init() = 0;

    /** Called when an update needs to be done */
    virtual void update() = 0;
    
    /** Gets a vector of all players from both teams. */
    PlayerVector players;
    /** Gets a vector of all opposition players. */
    PlayerVector opponents;
    /** Gets a vector of all team mates, including the running agent. */
    PlayerVector teamMates;
    /** Gets a vector of all landmarks, comprising four flags and four goal posts.
     * Note that this only contains goals and flags with objectIds of the form
     * Blah1Us, not Blah1Left. */
    ObjectVector landmarks;
    
    /** Gets localisation information about the running agent. */
    std::shared_ptr<bats::PlayerInfo> me;
    
    /** Gets localisation information about the ball. */
    std::shared_ptr<bats::DynamicObjectInfo> ball;
    
    /** Gets a vector of all objects. */
    ObjectVector objects;
    
    std::shared_ptr<bats::ObjectInfo> goal1Us;
    std::shared_ptr<bats::ObjectInfo> goal2Us;
    std::shared_ptr<bats::ObjectInfo> goal1Them;
    std::shared_ptr<bats::ObjectInfo> goal2Them;
    
    std::shared_ptr<bats::ObjectInfo> flag1Us;
    std::shared_ptr<bats::ObjectInfo> flag2Us;
    std::shared_ptr<bats::ObjectInfo> flag1Them;
    std::shared_ptr<bats::ObjectInfo> flag2Them;
    
    std::shared_ptr<bats::ObjectInfo> center;
    
    PlayerVector getFilteredPlayers(std::function<bool(std::shared_ptr<PlayerInfo>)> pred, bool aliveOnly = true);
    ObjectVector getFilteredObjects(std::vector<std::shared_ptr<ObjectInfo>> objects, std::function<bool(std::shared_ptr<ObjectInfo>)> pred);
    ObjectVector getAliveObjects(std::vector<std::shared_ptr<DynamicObjectInfo>> objects, std::function<bool(std::shared_ptr<DynamicObjectInfo>)> pred = 0);
    
    // TODO use std::function instead of template
    
    template <typename _Predicate, typename ObjectType>
    unsigned count(std::vector<std::shared_ptr<ObjectType>> objects, _Predicate pred)
    {
      unsigned count = 0;
      for (std::shared_ptr<ObjectType> object : objects)
      {
        if (bool(pred(object)))
          count++;
      }
      return count;
    };

    std::shared_ptr<ObjectInfo> getObjectById(Types::Object object) const { return d_objectInfos[object]; }

    /** Get the local transformation matrix
     *
     * This matrix describes the transformation from the local frame to the
     * agent frame, i.e. the axes of the agent frame in terms of the local
     * frame. Multiplication of a vector in the agent frame with this matrix
     * results in the position of that vector with respect to the local frame.
     */
    virtual Eigen::Transform3d getLocalTransformation() const = 0;

    /** Get the global transformation matrix
     *
     * This matrix describes the transformation from the global frame to the
     * agent frame, i.e. the axes of the agent frame in terms of the global
     * frame. Multiplication of a vector in the agent frame with this matrix
     * results in the position of that vector with respect to the global
     * frame.
     */
    virtual Eigen::Transform3d getGlobalTransformation() const = 0;
    
    /** 
     * @return a unit vector in the local frame's x/y plane that points in
     * the forward direction (towards the opponent's goal, parallel to the
     * sides of the field).
     */
    virtual Eigen::Vector3d getForwardDirLocal() const = 0;
    
    /** 
     * @return a unit vector in the local frame's x/y plane that points from
     * the left to the right of the field from the perspective of our goalie
     * looking down the field to the opponent's goal.  This vector is parallel
     * to the goal lines.
     */
    virtual Eigen::Vector3d getRightDirLocal() const = 0;

    /** @return the midpoint position of the opponent's goal in the local frame. The z-component is zero. */
    virtual Eigen::Vector3d getTheirGoalMidpointLocal() const = 0;
    /** @return the midpoint position of our team's goal in the local frame. The z-component is zero. */
    virtual Eigen::Vector3d getOurGoalMidpointLocal() const = 0;
    /** @return the midpoint position of the opponent's goal in the global frame. The z-component is zero. */
    virtual Eigen::Vector3d getTheirGoalMidpointGlobal() const = 0;
    /** @return the midpoint position of our team's goal in the global frame. The z-component is zero. */
    virtual Eigen::Vector3d getOurGoalMidpointGlobal() const = 0;

    /**
     * Function to tell the localizer that we know our position for sure,
     * for example after we have beamed to somewhere.
     */
    virtual void setGlobalPosition(Eigen::Vector3d &position) { };

    /** Add a new global measurement of an object to integrate */
    virtual void addGlobalMeasurement(std::shared_ptr<DynamicObjectInfo> dynamicObject, std::shared_ptr<Distribution> measurement) {};

    virtual void onBeam(std::shared_ptr<BeamEvent> event) {}

    /** Set camera offset due to calibration error */
    void setCameraOffset(Eigen::Vector3d const& offset)
    {
      d_cameraOffset = offset;
    }

    Eigen::Vector3d localToGlobal(Eigen::Vector3d const& loc, bool zeroZ = false)
    {
      Eigen::Vector3d global = Eigen::Transform3d(getGlobalTransformation() * getLocalTransformation().inverse()) * loc;
      if (zeroZ)
        global.z() = 0;
      return global;
    }

    Eigen::Vector3d globalToLocal(Eigen::Vector3d const& glob, bool zeroZ = false)
    {
      Eigen::Vector3d local = Eigen::Transform3d(getLocalTransformation() * getGlobalTransformation().inverse()) * glob;
      if (zeroZ)
        local.z() = 0;
      return local;
    }
      
    Eigen::Vector3d rotateLocalToGlobal(Eigen::Vector3d const& loc) { return Eigen::Transform3d(getGlobalTransformation().linear() * Eigen::Transform3d(getLocalTransformation().inverse()).linear()) * loc; }

    Eigen::Vector3d rotateGlobalToLocal(Eigen::Vector3d const& glob) { return Eigen::Transform3d(getLocalTransformation().linear() * Eigen::Transform3d(getGlobalTransformation().inverse()).linear()) * glob; }
  protected:
    std::shared_ptr<ObjectInfo> d_objectInfos[Types::NOBJECTS];
    
    /** The camera offset due to calibration error. */
    Eigen::Vector3d d_cameraOffset;
  };

  typedef Singleton<Localizer> SLocalizer;
}

#endif