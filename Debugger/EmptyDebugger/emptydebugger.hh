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
#ifndef BATS_EMPTYDEBUGGER_HH
#define BATS_EMPTYDEBUGGER_HH

#include "../debugger.hh"

namespace bats
{
  /** Dummy debugger
   *
   * You can initialize the Debugger singleton with this class to
   * disable all debugging.
   */
  class EmptyDebugger : public Debugger
  {
      friend class bats::Singleton<Debugger>;

    public:
      virtual void init(int argc, char** argv) {}
      virtual void start() {}
      virtual void run() {}
      
      virtual void reDraw() {}
      
      virtual void debug(std::string const& dbg) {}
      
      virtual void plot(std::string const& curve, float value) {}

      virtual void draw(std::shared_ptr<Shape> shape) {}
      virtual void drawForPeriod(std::shared_ptr<Shape> shape, double durationSeconds) {}
      virtual void drawUntil(std::shared_ptr<Shape> shape, double time) {}

      virtual bool isEnabled() { return false; }
      virtual bool isVerbose() { return false; }
      virtual void setVerbose(bool isVerbose) { }
      
    private:
  };

};


#endif /* BATS_EMPTYDEBUGGER_HH */

