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

#ifndef _BATS_RANGE_HH_
#define _BATS_RANGE_HH_

#include <utility>

namespace bats
{
  ///Defines a range between two values
  class Range: public std::pair<double, double> 
  {
  	public:

  	/** Construct a range 
      *
  	 * @param f Start value
  	 * @param s End value
  	 */
  	Range(double f, double s)
  		:
  		std::pair<double, double>(f, s)
  	{}
  		
    /** Construct a range with start = end = 0;
    */
  	Range()
  		:
  		std::pair<double, double>(0,0)
  	{}
  	
    /** copy constructor
    */
  	Range(Range const &other)
  		:
  		std::pair<double, double>(other.first, other.second)
  	{}
  	
  	/**
  	 * @returns the mean of the range: \f$ \mu \f$
  	 */
  	double mean()
  	{
  		return (first+second)/2.0;
  	}

  	/**
  	 * @returns the absolute mean of the range: <br>
  	 * \f$ \mu = {{max - min} \over 2} : if max \geq min\f$ <br>
  	 * \f$ \mu = -{{max - min} \over 2} : if max < min\f$ 
  	 */
  	double absMean()
  	{
  		return std::fabs(mean());
  	}
  }; 
}

#endif

