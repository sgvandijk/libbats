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
#ifndef BATS_PCSELECTOR_HH
#define BATS_PCSELECTOR_HH

#include "../Singleton/singleton.hh"

namespace bats
{
  /** Player class selector base class
   *
   * This class provides a basic interface to keep track of the player
   * class of the current agent. This can be used to define different
   * configurations and behavior for different agents.
   */
  class PCSelector
  {
  public:
    virtual ~PCSelector() {}

    /** Get the player class of the current agent
     *
     * This may change over time, based on the chosen player class
     * selector implementation.
    */
    virtual unsigned getPlayerClass() = 0;
    

    /** Set the player class of the current agent
     *
     * This may be ignored, based on the chosen player class selector
     * implementation.
    */
    virtual void setPlayerClass(unsigned c) {}
  };

  typedef Singleton<PCSelector> SPCSelector;
}

#endif
