/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:  Martin Klomp (martin@ai.rug.nl)
 *    Mart van de Sanden (vdsanden@ai.rug.nl)
 *    Sander van Dijk (sgdijk@ai.rug.nl)
 *    A. Bram Neijt (bneijt@gmail.com)
 *    Matthijs Platje (mplatje@gmail.com)
 *
 *  All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date:   November 1, 2008
 *
 *  Website:  http://www.littlegreenbats.nl
 *
 *  Comment:  Please feel free to contact us if you have any 
 *    problems or questions about the code.
 *
 *
 *  License:  This program is free software; you can redistribute 
 *    it and/or modify it under the terms of the GNU General
 *    Public License as published by the Free Software 
 *    Foundation; either version 3 of the License, or (at 
 *    your option) any later version.
 *
 *      This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A
 *    PARTICULAR PURPOSE.  See the GNU General Public
 *    License for more details.
 *
 *      You should have received a copy of the GNU General
 *    Public License along with this program; if not, write
 *    to the Free Software Foundation, Inc., 59 Temple Place - 
 *    Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef BATS_OBJECTINFO_HH
#define BATS_OBJECTINFO_HH

#include "../Types/types.hh"
#include "../Distribution/distribution.hh"
#include "../Distribution/NormalDistribution/normaldistribution.hh"

namespace bats
{
  /** Object Information
   *
   * Holds information about physical ojects, such as position and
   * velocity, whether it is seen, and some pysical properties.
   */
  struct ObjectInfo
  {
    /** Local position and velocity distribution
     *
     * Holds information about the object's position and velocity in the
     * local frame:
     * 
     * - a 6x1 Vector for mu, with the first 3 elements giving position, the
     *   last 3 elements giving velocity
     * - a 6x6 matrix for sigma, with the top-left 3x3 giving velocity
     */
    std::shared_ptr<Distribution> posVelLocal;

    /** Global position and velocity distribution
     *
     * Holds information about the object's position and velocity in the
     * global frame:
     * 
     * - a 6x1 Vector for mu, with the first 3 elements giving position, the
     *   last 3 elements giving velocity
     * - a 6x6 matrix for sigma, with the top-left 3x3 giving velocity
     */
    std::shared_ptr<Distribution> posVelGlobal;
    
    /** Raw position and velocity distribution
     *
     * Holds raw position and velocity data received from the simulator
     */
    std::shared_ptr<NormalDistribution> posVelRaw;

    /** Last global measurement
     *
     * Only used internally in KalmanLocalizer to determine velocity
     * \todo: make less specifically for KL
     */
    std::shared_ptr<NormalDistribution> lastPosVelRawGlobal;
    
    /** Whether this object was sighted in the last vision data. */
    bool isVisible;
    
    /** The Types::Object enum value that corresponds to this ObjectInfo. */
    const Types::Object objectId;
    
    /** Object radius */
    const double radius;
    
    /** Whether this object represents a player. */
    const bool isPlayer;
    /** Whether this object represents the ball. */
    const bool isBall;
    /** Whether this object represents the ball. */
    const bool isFlag;
    
    /** The name of this object. **/
    const std::string name;
    
    /** Whether this object is dynamic, in the sense that it can move around
     * the environment, in contrast to fixed objects such as flags and goals.
     */
    const bool isDynamic;

    ObjectInfo(const Types::Object objectId, const double radius, const bool isDynamic = false)
    : posVelLocal(std::make_shared<NormalDistribution>(6)),
      posVelGlobal(std::make_shared<NormalDistribution>(6)),
      posVelRaw(std::make_shared<NormalDistribution>(6)),
      lastPosVelRawGlobal(std::make_shared<NormalDistribution>(6)),
      isVisible(false),
      objectId(objectId), 
      radius(radius), 
      isPlayer(Types::isPlayer(objectId)),
      isBall(objectId == Types::BALL),
      isFlag(Types::isFlag(objectId)),
      name(Types::nameOf(objectId)),
      isDynamic(isDynamic)
    {}
    
    /** @return the most likely position of this object in the local frame of the agent. */
    Eigen::Vector3d getPositionLocal(const bool zeroZ = false) const
    {
      return setZeroZ(zeroZ, posVelLocal->getMu().head<3>());
    }
    
    /**
     * Gets the position distribution of this object in the local frame of the agent.
     * This represents the belief and uncertainty of the position.
     */
    std::shared_ptr<Distribution> getPositionDistributionLocal() const
    {
      std::shared_ptr<NormalDistribution> d = std::make_shared<NormalDistribution>(3);
      d->init(posVelLocal->getMu().head<3>(), posVelLocal->getSigma().block<3,3>(0,0));
      return d;
    }
    
    /** @return the most likely position of this object in the global frame. */
    Eigen::Vector3d getPositionGlobal(const bool zeroZ = false) const
    {
      return setZeroZ(zeroZ, posVelGlobal->getMu().head<3>());
    }
    
    /**
     * Gets the position distribution of this object in the global frame.
     * This represents the belief and uncertainty of the position.
     */
    std::shared_ptr<Distribution> getPositionDistributionGlobal() const
    {
      std::shared_ptr<bats::NormalDistribution> d = std::make_shared<NormalDistribution>(3);
      d->init(posVelGlobal->getMu().head<3>(), posVelGlobal->getSigma().block<3,3>(0,0));
      return d;
    }
    
    /** @return the unfiltered position of this object in the raw (camera) frame. */
    Eigen::Vector3d getPositionRaw() const
    {
      return posVelRaw->getMu().head<3>();
    }
    
    /** @return the most likely velocity of this object in the global frame. */
    Eigen::Vector3d getVelocityGlobal(const bool zeroZ = false) const
    {
      return setZeroZ(zeroZ, posVelGlobal->getMu().tail<3>());
    }
    
    /** @return the most likely velocity of this object in the local frame. */
    Eigen::Vector3d getVelocityLocal(const bool zeroZ = false) const
    {
      return setZeroZ(zeroZ, posVelLocal->getMu().tail<3>());
    }
    
    /** @return the most likely velocity of this object in the raw (camera) frame. */
    Eigen::Vector3d getVelocityRaw() const
    {
      return posVelRaw->getMu().tail<3>();
    }
    
  private:
    Eigen::Vector3d setZeroZ(const bool zeroZ, const Eigen::Vector3d vector) const
    {
      return Eigen::Vector3d(vector.x(), vector.y(), zeroZ ? 0 : vector.z());
    }
  };
}

#endif
