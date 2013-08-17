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
