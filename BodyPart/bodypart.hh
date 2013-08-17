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
#ifndef BATS_BODYPART_HH
#define BATS_BODYPART_HH

#include "../Distribution/NormalDistribution/normaldistribution.hh"
#include "../Types/types.hh"
#include <Eigen/Geometry>
#include <vector>

namespace bats {    
  /**
   * Body part information
   */
  struct BodyPart
  {
    virtual ~BodyPart() {}
    
    /// Name of body part
    std::string name;
    
    /** The transformation matrix of this body part.
     * 
     * This matrix contains the position and orientation of the body part
     * relative to the torso.
     * 
     * Using the returned Affine3d to transform a vector will convert
     * from the frame of this body part into the frame of the torso.
     */
    Eigen::Affine3d transform;

    Eigen::Vector3d getPosition()
    {
      return transform.translation().head<3>();
    }

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

  struct Joint;

  /**
   * Limb information
   */
  struct Limb : public BodyPart
  {
    /// Weight of limb
    double weight;
    
    /// Weight of limb divided by the robot's total weight
    double relativeWeight;
    
    /// Dimensions of limb
    Eigen::Vector3d size;
    
    /// List of joints attached to this limb
    std::vector<std::shared_ptr<Joint> > joints;

    // This bodypart's identifier
    Types::BodyPart id;

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
  
  /**
   * Joint information
   */
  struct Joint : public BodyPart
  {
    Joint()
      : rate(0), torque(0), control(0)
    {
      angle = std::make_shared<NormalDistribution>(1);
      angle->init(Eigen::VectorXd::Zero(1), Eigen::MatrixXd::Zero(1,1));
    }
    
    /// Axis of joint in local coordinate system
    Eigen::Vector3d axis;
    
    /// Name of joint's angle perceptor as used by server
    std::string perceptor;
    
    /// Name of joint's motor as used by server
    std::string effector;
    
    /// This joint's identifier
    Types::Joint id;
    
    /// This joint's type
    Types::JointType type;
    
    /// The index (0, 1) of this joint if it is part of a universal joint
    unsigned ujindex;

    /// Minimum and maximum angle bounds
    std::pair<double, double> bounds;

    /// Joint angle
    std::shared_ptr<NormalDistribution> angle;
    
    /// Joint rate, as given by server
    double rate;
    
    /// Torque action upon joint, as given by server
    double torque;
    
    /// Control velocity set at last time step
    double control;
    
    /// Body part connected by this joint
    std::shared_ptr<BodyPart> bodyPart;
    
    /// Anchor points of joint on body parts, relative to their center
    std::pair<Eigen::Vector3d, Eigen::Vector3d> anchors;
    
    /// @returns the joint's axis direction vector in the agent coordinate system
    Eigen::Vector3d getAxisVec() const
    {
      return transform * axis;
    }

    // Needed when having fixed sized Eigen member
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

}

#endif
