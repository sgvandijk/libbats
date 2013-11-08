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
    
    KalmanLocalizer();
    KalmanLocalizer(KalmanLocalizer const& other); //NI
    KalmanLocalizer& operator=(KalmanLocalizer const* other); //NI
    
    void updateSelfGlobal();
    void updateBallGlobal();
    void updatePlayersGlobal();
    void globalToLocal();
  };
}

#endif
