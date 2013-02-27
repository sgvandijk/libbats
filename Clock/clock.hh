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

#ifndef BATS_CLOCK_HH
#define BATS_CLOCK_HH

#include "../Singleton/singleton.hh"
#include <cmath>

namespace bats
{
  /** Clock base class
   */
  class Clock
  {
  public:
    virtual ~Clock() {}

    /** Update this clock
     *
     * Should update internal values to give current time.
     */
    virtual void update() = 0;

    /** The global time
     *
     * This is equivalent to the machine time, without a specified
     * reference time. So you should only use this time to determine
     * time differences. If you want the game time, use WorldModel
     * instead.
     */
    double getTime() const { return d_time; }

    /** The time difference between the current and the previous timestep */
    double getDt() const { return d_dt; }

    /** Check whether given time is the previous time step */
    bool isPreviousTimeStep(double time) { return fabs(time + d_dt - d_time) < 1e-3; }

  protected:
    double d_time;
    double d_dt;
  };

  typedef Singleton<Clock> SClock;
}

#endif // BATS_CLOCK_HH
