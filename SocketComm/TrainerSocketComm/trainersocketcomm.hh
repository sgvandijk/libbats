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
#ifndef __INC_BATS_TRAINERSOCKETCOMM_HH_
#define __INC_BATS_TRAINERSOCKETCOMM_HH_

#include <Eigen/Core>

#include "../socketcomm.hh"
#include "../../Types/types.hh"
#include "../../Singleton/singleton.hh"

namespace bats
{
  /**
   * Specialized SocketComm to help a trainer communicate with the server
   */
  class TrainerSocketComm : public SocketComm
  {
    friend class Singleton<TrainerSocketComm>;

    TrainerSocketComm(TrainerSocketComm const&); //NI
    TrainerSocketComm& operator=(TrainerSocketComm const&); //NI
    
    TrainerSocketComm()
    {}

  public:

    void init();

    static std::shared_ptr<Predicate> makeInitMessage();
    
    void setBallPos(Eigen::Vector3d const& pos);
    
    static std::shared_ptr<Predicate> makeSetBallPosMessage(Eigen::Vector3d const& pos);
    
    void setBallVel(Eigen::Vector3d const& vel);
    
    static std::shared_ptr<Predicate> makeSetBallVelMessage(Eigen::Vector3d const& vel);
    
    void setBallPosVel(Eigen::Vector3d const& pos, Eigen::Vector3d const& vel);
    
    static std::shared_ptr<Predicate> makeSetBallPosVelMessage(Eigen::Vector3d const& pos, Eigen::Vector3d const& vel);
    
    void kickOff();
    
    static std::shared_ptr<Predicate> makeKickOffMessage();
    
    void setPlayMode(Types::PlayMode mode);
    
    static std::shared_ptr<Predicate> makeSetPlayModeMessage(Types::PlayMode mode);
    
    static unsigned port;
  };
  
  typedef Singleton<TrainerSocketComm> STrainerSocketComm;
};

#endif
