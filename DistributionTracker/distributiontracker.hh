/*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:  Martin Klomp (martin@ai.rug.nl)
 *    Mart van de Sanden (vdsanden@ai.rug.nl)
 *    Sander van Dijk (sgdijk@ai.rug.nl)
 *    A. Bram Neijt (bneijt@gmail.com)
 *    Matthijs Platje (mplatje@gmail.com)
 *
 *  All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date:   November 1, 2008
 *
 *  Website:  http://www.littlegreenbats.nl
 *
 *  Comment:  Please feel free to contact us if you have any 
 *    problems or questions about the code.
 *
 *
 *  License:  This program is free software; you can redistribute 
 *    it and/or modify it under the terms of the GNU General
 *    Public License as published by the Free Software 
 *    Foundation; either version 3 of the License, or (at 
 *    your option) any later version.
 *
 *      This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A
 *    PARTICULAR PURPOSE.  See the GNU General Public
 *    License for more details.
 *
 *      You should have received a copy of the GNU General
 *    Public License along with this program; if not, write
 *    to the Free Software Foundation, Inc., 59 Temple Place - 
 *    Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef _BATS_DISTRIBUTIONTRACKER_HH_
#define _BATS_DISTRIBUTIONTRACKER_HH_

#include <Eigen/Core>

namespace bats
{
  class DistributionTracker
  {
  public:
    void reset() { d_total = d_totalSquared = d_count = 0; };
    const unsigned count() { return d_count; }
    
    void add(double d)
    {
      if (std::isnan(d))
        throw std::runtime_error("Cannot add nan to a DistributionTracker");
      d_total += d;
      d_totalSquared += d*d;
      d_count++;
    }
    
    const double average() const
    {
      return d_count == 0 ? NAN : d_total/d_count;
    }
    
    const double stdDev() const
    {
      double avg = average();
      return d_count == 0 ? NAN : d_totalSquared/d_count - avg*avg;
    }
      
  private:
    double d_total;
    double d_totalSquared;
    unsigned d_count;
  };

  class Vector3dDistributionTracker
  {
  public:
    void reset()
    { 
      d_x.reset();
      d_y.reset();
      d_z.reset();
    };
    
    const unsigned count() { return d_x.count(); }
    
    void add(Eigen::Vector3d vector)
    {
      d_x.add(vector.x());
      d_y.add(vector.y());
      d_z.add(vector.z());
    }
    
    const Eigen::Vector3d average() const
    {
      return Eigen::Vector3d(d_x.average(), d_y.average(), d_z.average());
    }
    
    const Eigen::Vector3d stdDev() const
    {
      return Eigen::Vector3d(d_x.stdDev(), d_y.stdDev(), d_z.stdDev());
    }
      
  private:
    DistributionTracker d_x;
    DistributionTracker d_y;
    DistributionTracker d_z;
  };  
}

#endif /* _BATS_DISTRIBUTIONTRACKER_HH_ */