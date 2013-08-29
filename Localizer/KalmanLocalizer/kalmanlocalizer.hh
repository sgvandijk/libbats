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
#ifndef BATS_KALMANLOCALIZER_HH
#define BATS_KALMANLOCALIZER_HH

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
  /** Kalman filter localizer implementation
   *
   * @ingroup localization
   */
  class KalmanLocalizer : public Localizer
  {
  public:
    virtual void init() override;
    virtual void update() override;

    virtual void addGlobalMeasurement(std::shared_ptr<DynamicObjectInfo> dynamicObject,
                                      std::shared_ptr<Distribution> measurement) override;
    
    void onBeam(std::shared_ptr<BeamEvent> event);

    virtual Eigen::Vector3d getTheirGoalMidpointLocal() const override;

    virtual Eigen::Vector3d getOurGoalMidpointLocal() const override;

    virtual Eigen::Vector3d getTheirGoalMidpointGlobal() const override;

    virtual Eigen::Vector3d getOurGoalMidpointGlobal() const override;
    
    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    friend class Singleton<Localizer>;
    
    typedef std::pair<std::shared_ptr<bats::ObjectInfo>, std::shared_ptr<bats::ObjectInfo>> LandmarkPair;
    typedef std::pair<LandmarkPair, LandmarkPair> LandmarkPairPair;

    double d_lastRotationCheck;
    std::vector<LandmarkPair> d_forwardPairs;
    std::vector<LandmarkPair> d_rightPairs;
    std::vector<LandmarkPairPair> d_rightCrossProductPairs;
    
    Eigen::Affine3d d_globalRotation;
    Eigen::Affine3d d_globalTranslation;
    
    // True if we received vision data this cycle.
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

    Cochlea::InfoID getCochleaIdForObject(std::shared_ptr<ObjectInfo> object) const;
  };

  Eigen::Vector3d KalmanLocalizer::cutPositionVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.head<3>();
  }
  
  Eigen::Vector3d KalmanLocalizer::cutVelocityVector(Eigen::VectorXd const& posVel) const
  {
    return posVel.tail<3>();
  }

  void KalmanLocalizer::setPositionVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& pos)
  {
    posVel.head<3>() = pos;
  }
  
  void KalmanLocalizer::setVelocityVector(Eigen::VectorXd& posVel, Eigen::Vector3d const& vel)
  {
    posVel.tail<3>() = vel;
  }
  
  Eigen::Matrix3d KalmanLocalizer::cutPositionMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.topLeftCorner<3,3>();
  }

  Eigen::Matrix3d KalmanLocalizer::cutVelocityMatrix(Eigen::MatrixXd const& posVel) const
  {
    return posVel.bottomRightCorner<3,3>();
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
    locVel.topLeftCorner<3,3>() = loc;
    locVel.bottomRightCorner<3,3>() = vel;
    return locVel;
  }
}

#endif
