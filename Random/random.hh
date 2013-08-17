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
#ifndef __BATS_RANDOM_HH__
#define __BATS_RANDOM_HH__

#include <Eigen/Core>

namespace bats
{
  /** Utility class to work with random numbers */
  class Random
  {
    public:
      /// Seed the random number generator
      static void seed(unsigned s);
      
      /// Draw a value from a uniform distribution over [@a begin, @a end)
      static double uniform(double begin = 0, double end = 1);
      
      /// Create a random vector, where each element is drawn uniformally from the range defined by @a begin and @a end
      static Eigen::VectorXd uniform(Eigen::VectorXd const& begin, Eigen::VectorXd const& end);
      
      /// Draw a value from a standard normal distribution
      static double stdNorm();
      
    private:
      Random (); // NI

  };
}


#endif /* __BATS_RANDOM_HH__ */

