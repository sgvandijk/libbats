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
#ifndef BATS_DISTRIBUTIONTRACKER_HH
#define BATS_DISTRIBUTIONTRACKER_HH

#include <Eigen/Core>

namespace bats
{
  /** Track the statistics of a time series
   *
   * This class allows you to gather statistics of samples, by adding
   * them iteratively. The average and standard deviation are updated
   * accordingly.
   */
  class DistributionTracker
  {
  public:
    /** Reset statistics
     *
     * Sets mean and variation to 0
     */
    void reset();

    /**
     * @returns number of values added
     */
    unsigned count() const;
    
    /** Add new sample
     */
    void add(double d);
    
    /**
     * @returns the current average of all added samples
     */
    double average() const;
    
    /**
     * @returns the current variance of all added samples
     */
    double variance() const;
      
  private:
    double d_total;
    double d_totalSquared;
    unsigned d_count;
  };

  /** Track the statistics of a 3-dimensional time series
   *
   * This class allows you to gather statistics of vector samples, by adding
   * them iteratively. The average and standard deviation are updated
   * accordingly.
   */
  class Vector3dDistributionTracker
  {
  public:
    /** Reset statistics
     *
     * Sets mean and variation to (0,0,0)
     */
    void reset();
    
    /**
     * @returns number of samples added so far
     */
    unsigned count() const;
    
    /** Add a new sample
     */
    void add(Eigen::Vector3d vector);
    
    /**
     * @returns the current average of all added samples
     */
    Eigen::Vector3d average() const;
    
    /**
     * @returns the current element-wise variance of all added samples
     */
    Eigen::Vector3d variance() const;
      
  private:
    DistributionTracker d_x;
    DistributionTracker d_y;
    DistributionTracker d_z;
  };  

  // Inline member implementations
  
  // DistributionTracker

  inline void DistributionTracker::reset()
  {
    d_total = d_totalSquared = d_count = 0;
  }

  inline unsigned DistributionTracker::count() const
  {
    return d_count;
  }
    
  inline void DistributionTracker::add(double d)
  {
    if (std::isnan(d))
      throw std::runtime_error("Cannot add nan to a DistributionTracker");
    d_total += d;
    d_totalSquared += d*d;
    d_count++;
  }
    
  inline double DistributionTracker::average() const
   {
     return d_count == 0 ? NAN : d_total/d_count;
   }
    
  inline double DistributionTracker::variance() const
  {
    double avg = average();
    return d_count == 0 ? NAN : d_totalSquared/d_count - avg*avg;
  }

  // Vector3dDistributionTracker

  inline void Vector3dDistributionTracker::reset()
  { 
    d_x.reset();
    d_y.reset();
    d_z.reset();
  }
    
  inline unsigned Vector3dDistributionTracker::count() const
  {
    return d_x.count();
  }
    
  inline void Vector3dDistributionTracker::add(Eigen::Vector3d vector)
  {
    d_x.add(vector.x());
    d_y.add(vector.y());
    d_z.add(vector.z());
  }
    
  inline Eigen::Vector3d Vector3dDistributionTracker::average() const
  {
    return Eigen::Vector3d(d_x.average(), d_y.average(), d_z.average());
  }
    
  inline Eigen::Vector3d Vector3dDistributionTracker::variance() const
  {
    return Eigen::Vector3d(d_x.variance(), d_y.variance(), d_z.variance());
  }

}

#endif /* BATS_DISTRIBUTIONTRACKER_HH */
