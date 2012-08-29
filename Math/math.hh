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

#ifndef __INC_BATS_MATH_HH_
#define __INC_BATS_MATH_HH_

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace bats
{
  /** Math has a few helpful mathematical functions
    */
  class Math
  {
  public:

   /** Calculates the distance from a given point to the closest point on a given line
     *
     * To calculate the distance between a point \f$(x, y)\f$ and a line starting at <br>
     * point \f$(l_{x1}, l_{y1})\f$ with a normalized vector \f$(l_{x2}, l_{y2})\f$: <br>
     *
     * First calculate the perpendicular of the normalized vector, resulting in \f$(p_x, p_y)\f$ <br>
     * The result is then given by the length of \f$(l_{x2}, l_{y2} * u) - (x, y)\f$ where \f$u\f$ equals
     *
     * \f$u = (\frac{p_x}{p_y} * (l_{x2} - y) + (x - l_{x1})) / (l_{x2} - (\frac{p_x}{p_y} * l_{y2}))\f$
     *
     * @param l0 the starting point of the line.
     * @param lVect the normalized vector of the line.
     * @param point The point.
     * @return The distance from the line to the point
     */
    static double distanceLinePoint(Eigen::Vector3d const &l0,
                                    Eigen::Vector3d const &lVect,
                                    Eigen::Vector3d const &point);

    /**
     * Calculates the nearest point on a given line to a given point.
     *
     * First determine wether a perpendicular line between the point and the line exists. <br>
     * If there is such a line, this will give the nearest point on the line.
     *
     * To calculate the distance between a point \f$(x, y)\f$ and a line starting at <br>
     * point \f$(l_{x1}, l_{y1})\f$ with a normalized vector \f$(l_{x2}, l_{y2})\f$: <br>
     *
     * First calculate the perpendicular of the normalized vector, resulting in \f$(p_x, p_y)\f$ <br>
     * The result is then given by the length of \f$(l_{x2}, l_{y2} * u) - (x, y)\f$ where \f$u\f$ equals
     *
     * \f$u = (\frac{p_x}{p_y} * (l_{x2} - y) + (x - l_{x1})) / (l_{x2} - (\frac{p_x}{p_y} * l_{y2}))\f$
     *
     * When no perpendicular line is posible within the segment, use the closest endpoint of the line.
     *
     * @param l0 The starting point of the line.
     * @param lVect The line direction vector.
     * @param point The other point.
     * @param segment If true, only consider points on the segment [@l0, @l0 + @lVect], otherwise use full line @l0 + \lambda @lVect
     * @return The point on the line closest to the given point
     */
    static Eigen::Vector3d linePointClosestToPoint(Eigen::Vector3d const &l0,
                        Eigen::Vector3d const &lVect,
                        Eigen::Vector3d const &point,
                        bool segment = true);

    /**
     * Calculates the perpendicular vector to \a v.
     *
     * The perpendicular vector to \f$v = (x, y)\f$ is given by \f$(1.0, -1.0 / \frac{x}{y})\f$
     *
     * @param v A vector
     * @return The perpendicular vector to v
     */
    static Eigen::Vector3d calcPerpend(Eigen::Vector3d const &v);

    /** Calculate the point where a given line intersects a given plane
     *
     * To calculate the point where a line with starting position \f$(l_{x1}, l_{y1}, l_{z1})\f$ and <br>
     * direction \f$(l_{x2}, l_{y2}, l_{z2})\f$ intersects a plane \f$(a, b, c, d)\f$:
     *
     * \f$x = l_{x1} + f * l_{x2}\f$
     *
     * \f$a * x + b * y + c * z + d = 0\f$ <br>
     * \f$a * l_{x1} + a * f * l_{x2} + b * l_{y1} + b * f * l_{y2} + c * l_{z1} + c * f * l_{z2} + d = 0\f$
     *
     * \f$f * (a * l_{x2} + b * l_{y2} + c * l_{z2}) = - a * l_{x1} - b * l_{y1} - c * l_{z1} - d\f$ <br>
     * \f$f = (- a * l_{x1} - b * l_{y1} - c * l_{z1} - d) / (a * l_{x2} + b * l_{y2} + c * l_{z2})\f$
     *
     * The result is than given by the vector: (position + direction * \f$f\f$)
     *
     * @param position The starting point of the line
     * @param direction The direction of the line
     * @param plane The plane to intersect
     * @return The position where the line intersects the plane
     */
    static Eigen::Vector3d intersectVectorPlane(Eigen::Vector3d const& position, Eigen::Vector3d const& direction, Eigen::Vector4d const& plane);

    /** Determine the point where two lines intersect in the X,Y plane
     *
     * @param pos1 A point on the first line
     * @param dir1 The direction of the first line
     * @param pos2 A point on the second line
     * @param dir2 The direction of the second line
     */
    static Eigen::Vector3d intersectLines2D(Eigen::Vector3d const& pos1, Eigen::Vector3d const& dir1, Eigen::Vector3d const& pos2, Eigen::Vector3d const& dir2);

    /** Determine the point where a line intersects a circle
     * 
     * @param position A point on the line
     * @param direction The direction of the line
     * @param center Center of circle
     * @param radius Radius of the circle
     */
    static bool intersectLineCircle(Eigen::Vector3d& intersection, Eigen::Vector3d const& position, Eigen::Vector3d const& direction, Eigen::Vector3d const& center, double radius);
    
    /** Convert polar coordinates to cartesian coordinates
     *
     */
    static Eigen::Vector3d polarToCartesian(Eigen::Vector3d const &polar);

    /** Convert cartesian coordinates to polar coordinates
     *
     */
    static Eigen::Vector3d cartesianToPolar(Eigen::Vector3d const &cartesian);

    /** Convert degrees to radians */
    static double degToRad(double deg)
    {
      return deg / 180.0 * M_PI;
    }

    /** Convert radians to degrees */
    static double radToDeg(double rad)
    {
      return rad / M_PI * 180.0;
    }

    /** Normalize an angle in radians to the range [-pi, pi] */
    static double normalizeRadBalanced(double angle)
    {
      angle = fmod(angle, 2.0 * M_PI);
      if (angle > M_PI)
        angle -= 2.0 * M_PI;
      else if (angle < -M_PI)
        angle += 2.0 * M_PI;
      return angle;
    }

    /** Normalize an angle in radians to the range [0, 2*pi] */
    static double normalizeRadPositive(double angle)
    {
      return fmod(angle, 2.0 * M_PI);
    }

    /** Determine whether 2 points are on the same side of a third point
      *
      * @param pivot Reference point
      * @param a First point
      * @param b Second point
      * @returns whether @a a and @a b are on the same side of @a pivot
      */
    static bool atSameSideOf(Eigen::Vector3d const& pivot, Eigen::Vector3d const& a, Eigen::Vector3d const& b);

    /** Gaussian kernel function
     *
     * \f$ kernel(d, h) = e^{-\frac{d}{h}} \f$
     */
    static double kernel(double distSqr, double bandWidthSqr)
    {
      return exp(-distSqr/bandWidthSqr);
    }

    /** Gaussian kernel function
     *
     * \f$ kernel(d, h) = e^{-\frac{d}{h}} \f$
     */
    static double kernel(Eigen::Vector3d const& v, double bandWidth)
    {
      return kernel(v.squaredNorm(), bandWidth * bandWidth);
    }
    
    /**
     *  A sigmoid function, that starts at @f$ y = start @f$ and ends at @f$ y = end @f$, @f$(0 < x < 1)@f$.
     *
     *  @param x the position in time or distance or etc. running from 0.0 to 1.0.
     *  @param start the starting output at @f$ x = 0.0 @f$.
     *  @param end the ending output at @f$ x = 1.0 @f$.
     *  @param c this parameter specifies the speedness of the sigmoid, higher is more steeper, lower is less steep.
     */
    static double sigmoid(double x, double start, double end, double c = 8)
    {
      return (1.0 - (1.0/(1+exp((0.5-x)*c))))*(start-end) + end;
    }

    /**
     *  @returns the quadratic decay, @f$ 1.0 - x^2 @f$, at time index @f$ x @f$, @f$ (0<x<1) @f$.
     */
    static double quadraticDecay(double x)
    {
      return 1.0-x*x;
    }

    /** Build a transfomration matrix out of direction vectors
     *
     * @param right Direction of the x-axis
     * @param forward Direction of the y-axis
     * @param up Direction of the z-axis
     * @param trans Translation
     */
    static Eigen::Affine3d makeTransform(Eigen::Vector3d const& right, Eigen::Vector3d const& forward, Eigen::Vector3d const& up, Eigen::Vector3d const& trans = Eigen::Vector3d(0,0,0))
    {
      Eigen::Matrix4d mat;
      mat << right.transpose(), 0,
             forward.transpose(), 0,
             up.transpose(), 0,
             trans.transpose(), 1;
      return Eigen::Affine3d(mat.transpose());
    }

    /** Make a rotation matrix given a vector of Euler angles
     *
     * @param angles Vector giving the rotation around x, y and z-axis in radians
     */
    static Eigen::Affine3d makeRotation(Eigen::Vector3d const angles);

    /** Get the direction of the x-axis of a transformation
     */
    static Eigen::Vector3d getRight(Eigen::Affine3d const& trans)
    {
      return trans.matrix().col(0).head<3>();
    }

    /** Get the direction of the y-axis of a transformation
     */
    static Eigen::Vector3d getForward(Eigen::Affine3d const& trans)
    {
      return trans.matrix().col(1).head<3>();
    }

    /** Get the direction of the z-axis of a transformation
     */
    static Eigen::Vector3d getUp(Eigen::Affine3d const& trans)
    {
      return trans.matrix().col(2).head<3>();
    }

    /** Get the translation part of a transformation
     */
    static Eigen::Vector3d getTrans(Eigen::Affine3d const& trans)
    {   // in the original bats::Vector3D this function was called trans
      return trans.matrix().col(3).head<3>();
    }

    /** Get the angle between two vectors
     */
    static double getAngle(Eigen::Vector3d const &v1, Eigen::Vector3d const & v2)
    {
        return acos(v1.normalized().dot(v2.normalized()));
    }

    /** Get the difference between two angles, normalized to the range @f$ [-\pi, \pi] @f$
     */
    static double angleDiff(double a, double b)
    {
      double diff = fmod(b - a, 2.0 * M_PI);
      if (diff > M_PI)
        diff -= 2.0 * M_PI;
      return diff;
    }
  };

};

#endif // __INC_BATS_MATH_HH_
