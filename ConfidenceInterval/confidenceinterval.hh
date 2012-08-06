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

#ifndef _BATS_CONFIDENCEINTERVAL_HH_
#define _BATS_CONFIDENCEINTERVAL_HH_

#include <utility>

namespace bats
{
  struct ConfidenceInterval: public std::pair<double, double>
  {
    /** Initialize a new confidence interval with the given value and variance
        @param value
        @param variance
    */
    ConfidenceInterval(double value, double variance)
      :
        std::pair<double,double>(value, variance)
      {}

    /** Construct a new confidence interval with value 0 and variance 0
    */
    ConfidenceInterval()
      :
        std::pair<double,double>(0,0)
      {}

    /** Rounds value and variance to n decimals
        @param n number of decimals, in the range 1-4 (default: 2)
    */
    void round(size_t dec = 2)
    {
      double f = 10;
      switch(dec)
      {
        case 4: f *= f;//falling through
        case 3: f *= f;//falling through
        case 2: f *= f;
      }

      first = ::round(first*f)/f;
      second = ::round(second*f)/f;
    }
  };
}

#endif

