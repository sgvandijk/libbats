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
#ifndef __BATS_PLAYERCLASS_HH_
#define __BATS_PLAYERCLASS_HH_

#include "../Conf/conf.hh"
#include <map>
#include <memory>

namespace bats
{
  /** Player class
   *
   * Base class for implementing different classes that players can belong to.
   */
  class PlayerClass
  {
  public:
    /// Maps an index to a player class
    typedef std::map<unsigned, std::shared_ptr<PlayerClass> > PCMap;
    
    PlayerClass(unsigned idx);
    
    /// Creates playerclasses from configuration file
    template<typename T>
    static void createPlayerClasses();
    
    /// Get all created playerclasses
    static PCMap getPlayerClasses() { return s_playerClasses; }
    
    /// Get the player class with the given index
    static std::shared_ptr<PlayerClass> getPlayerClass(unsigned idx) { return s_playerClasses[idx]; }

  protected:
    unsigned d_idx;
    
    static PCMap s_playerClasses;
  };
  

  // Member implementations
  inline PlayerClass::PlayerClass(unsigned idx)
    : d_idx(idx)
  {
  }

  template<typename T>
  void PlayerClass::createPlayerClasses()
  {
    Conf& conf = SConf::getInstance();
    
    XMLNodeSet PCNS = conf.selectXPath("/conf/player-class");
    for (XMLNodeSet::iterator iter = PCNS.begin(); iter != PCNS.end(); ++iter)
    {
      XMLNode PCNode = *iter;
      if (!PCNode.hasProp("index"))
      {
        continue;
      }
      
      unsigned idx = atoi(PCNode.getProp("index").c_str());
      std::shared_ptr<PlayerClass> pc = s_playerClasses[idx] = std::make_shared<T>(idx);
    }
  }
  

}

#endif
