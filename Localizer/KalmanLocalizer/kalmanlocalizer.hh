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

#ifndef _BATS_KALMANLOCALIZER_HH_
#define _BATS_KALMANLOCALIZER_HH_

#include "../localizer.hh"
#include "../../Distribution/NormalDistribution/normaldistribution.hh"
#include "../../BatsEvent/batsevent.hh"
#include "../../Cochlea/cochlea.hh"
#include <map>
#include <vector>
#include <utility>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace bats
{
  class KalmanLocalizer : public Localizer
  {
  public:
    virtual void init();
    virtual void update();

    void resetBall();

    /** Gets transformation from agent (torso) space to local space, including both translation and rotation. */
    virtual inline Eigen::Transform3d getLocalTransformation() const;

    /** Gets transformation from agent (torso) space to global space, including both translation and rotation. */
    virtual inline Eigen::Transform3d getGlobalTransformation() const;
    
    virtual void addGlobalMeasurement(rf<DynamicObjectInfo> dynamicObject, rf<Distribution> measurement);
    
    void onBeam(rf<BeamEvent> event);

    /**
     * @return a unit vector in the in the local frame's x/y plane that points in
     * the forward direction (towards the opponent's goal, parallel to the sides
     * of the field). The z-component is zero.
     */
    virtual Eigen::Vector3d getForwardDirLocal() const;

    /** 
     * @return a unit vector in the local frame's x/y plane that points from the
     * left to the right of the field from the perspective of our goalie looking
     * down the field to the opponent's goal.  This vector is parallel to the
     * goal lines. The z-component is zero.
     */    
    virtual Eigen::Vector3d getRightDirLocal() const;

    /** @return the midpoint position of the opponent's goal in the local frame. The z-component is zero. */
    virtual Eigen::Vector3d getTheirGoalMidpointLocal() const;
    /** @return the midpoint position of our team's goal in the local frame. The z-component is zero. */
    virtual Eigen::Vector3d getOurGoalMidpointLocal() const;
    /** @return the midpoint position of the opponent's goal in the global frame. The z-component is zero. */
    virtual Eigen::Vector3d getTheirGoalMidpointGlobal() const;
    /** @return the midpoint position of our team's goal in the global frame. The z-component is zero. */
    virtual Eigen::Vector3d getOurGoalMidpointGlobal() const;
    
    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    friend class Singleton<Localizer>;
    
    typedef std::pair<bats::rf<bats::ObjectInfo>, bats::rf<bats::ObjectInfo>> LandmarkPair;
    typedef std::pair<LandmarkPair, LandmarkPair> LandmarkPairPair;

    double d_lastRotationCheck;
    std::vector<LandmarkPair> d_forwardPairs;
    std::vector<LandmarkPair> d_rightPairs;
    std::vector<LandmarkPairPair> d_rightCrossProductPairs;
    
    /** Transformation from agent (torso) space to global space, excluding translation. */
    Eigen::Transform3d d_globalRotation;
    /** Transformation from agent (torso) space to global space, excluding rotation. */
    Eigen::Transform3d d_globalTranslation;
    /** Transformation from agent (torso) space to global space, including both translation and rotation. */
    Eigen::Transform3d d_globalTransform;
    /** Transformation from agent (torso) space to local space, including both translation and rotation. */
    Eigen::Transform3d d_localTransform;
    
    /** True if we received vision data this cycle. */
    bool d_haveNewVisionData;
    
    KalmanLocalizer();
    KalmanLocalizer(KalmanLocalizer const& other); //NI
    KalmanLocalizer& operator=(KalmanLocalizer const* other); //NI

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
    void updateSelfGlobal();
    void updateBallGlobal();
    void updatePlayersGlobal();
    void globalToLocal();

    Cochlea::InfoID getCochleaIdForObject(rf<ObjectInfo> object) const;
  };

  Eigen::Transform3d KalmanLocalizer::getLocalTransformation() const
  {
    return d_localTransform;
  }

  Eigen::Transform3d KalmanLocalizer::getGlobalTransformation() const
  {
    return d_globalTransform;
  }

  Eigen::Vector3d KalmanLocalizer::cutPositionVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.start<3>();
  }
  
  Eigen::Vector3d KalmanLocalizer::cutVelocityVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.end<3>();
  }

  void KalmanLocalizer::setPositionVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& pos)
  {
    posVel.start<3>() = pos;
  }
  
  void KalmanLocalizer::setVelocityVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& vel)
  {
    posVel.end<3>() = vel;
  }
  
  Eigen::Matrix3d KalmanLocalizer::cutPositionMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.corner<3,3>(Eigen::TopLeft);
  }

  Eigen::Matrix3d KalmanLocalizer::cutVelocityMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.corner<3,3>(Eigen::BottomRight);
  }

  Eigen::VectorXd KalmanLocalizer::joinPositionAndVelocityVectors(Eigen::Vector3d const& loc, Eigen::Vector3d const& vel) const
  {
    Eigen::VectorXd locVel(6);
    locVel << loc, vel;
    return locVel;
  }

  Eigen::MatrixXd KalmanLocalizer::joinPositionAndVelocityMatrices(Eigen::Matrix3d const& loc, Eigen::Matrix3d const& vel) const
  {
    Eigen::MatrixXd locVel = Eigen::MatrixXd::Zero(6,6);
    locVel.fill(0);
    locVel.corner<3,3>(Eigen::TopLeft) = loc;
    locVel.corner<3,3>(Eigen::BottomRight) = vel;
    return locVel;
  }
}

#endif