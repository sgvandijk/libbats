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
#ifndef __BATS_DEBUGGER_HH__
#define __BATS_DEBUGGER_HH__

#include "../Singleton/singleton.hh"
#include "../BatsEvent/batsevent.hh"

#include "../Shape/shape.hh"

#include <queue>
#include <sstream>

namespace bats
{
  /** Base class for debuggers
   *
   * @ingroup debugging
   */
  class Debugger
  {
  public:
    /** A plottable curve */
    struct Curve
    {
      Curve() : minX(0), maxX(0), minY(0), maxY(0) {}
      
      std::vector<float> data;
      float minX;
      float maxX;
      float minY;
      float maxY;
    };
    
    virtual ~Debugger() {}

    /** Initialize debugger. */
    virtual void init(int argc, char** argv) = 0;

    /** Start debugger. */
    virtual void start() = 0;
    
    /** Redraw all debug output. */
    virtual void reDraw() = 0;
    
    /** Show a debug string. */
    virtual void debug(std::string const& dbg) = 0;
    
    /** Plot a new value for a named curve. */
    virtual void plot(std::string const& curve, float value) = 0;
    
    /** Draw a shape for a single timestep. */
    virtual void draw(std::shared_ptr<Shape> shape) = 0;

    /** Draw a shape for a given duration */
    virtual void drawForPeriod(std::shared_ptr<Shape> shape, double durationSeconds) = 0;    
    /** Draw a shape up until a given time */
    virtual void drawUntil(std::shared_ptr<Shape> shape, double time) = 0; 
    
    /** Whether debugging is enabled.
     *
     * If this value is false, then calls to other methods on this
     * debugger have no effect.
     */
    virtual bool isEnabled() = 0;
    
    /** Whether verbose debugging is enabled
     * 
     * Get whether we're showing all debug output, or only a selection
     * (based on the player in possession, for example.)
     */
    virtual bool isVerbose() = 0;
    
    /** Set whether verbose debugging is enabled
     * 
     * @param isVerbose Determines whether we're showing all debug
     * output, or only a selection (based on the player in possession,
     * for example.)
     */
    virtual void setVerbose(bool isVerbose) = 0;
  };
  
  template<typename T>
  Debugger& operator<<(Debugger& debugger, T const& dbg)
  {
    std::ostringstream out;
    out << dbg;
    debugger.debug(out.str());
    return debugger;
  }
  
  typedef Singleton<Debugger> SDebugger;
};


#endif /* __BATS_DEBUGGER_HH__ */

