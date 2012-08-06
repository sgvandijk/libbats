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

#ifndef __BATS_RC3DACTION_HH_
#define __BATS_RC3DACTION_HH_

#include "../action.hh"
#include "../../Types/types.hh"
#include <Eigen/Core>

namespace bats
{
  struct RC3DAction : public Action
  {
    enum RC3DActionType
    {
      NONE,               ///< No action (only used internally)
      MOVEJOINT,          ///< Move a joint (hinge joint or part of a universal joint)
      MOVEHINGEJOINT,     ///< Move a hinge joint
      MOVEUNIVERSALJOINT, ///< Move a universal joint
      BEAM,               ///< Beam
      SAY                 ///< Say
    };
    
    RC3DAction(RC3DActionType type)
    : Action(type)
    {}
  };

  /// Move a joint (hinge joint or part of a universal joint)
  struct MoveJointAction : public RC3DAction
  {
    /// The joint (hinge joint or part of a universal joint) to move
    Types::Joint joint;
          
    /// The angular velocity to move the joint with (radians per second)
    double speed;
          
    MoveJointAction(Types::Joint j, double s)
      : RC3DAction(MOVEJOINT), joint(j), speed(s)
    {}
    
  };

  /// Move a hinge joint
  struct MoveHingeJointAction : public RC3DAction
  {
    /// The hinge joint to move
    Types::Joint joint;
          
    /// The angular velocity to move the joint with (radians per second)
    double speed;
          
    MoveHingeJointAction(Types::Joint j, double s)
      : RC3DAction(MOVEHINGEJOINT), joint(j), speed(s)
    {}
  };
      
  /// Move a universal joint
  struct MoveUniversalJointAction : public RC3DAction
  {
    /// The universal joint to move
    Types::Joint joint;
          
    /// The angular velocity to move the joint with along its first axis (radians per second) 
    double speed1;
          
    /// The angular velocity to move the joint with along its second axis (radians per second)
    double speed2;
          
    MoveUniversalJointAction(Types::Joint j, double s1, double s2)
      : RC3DAction(MOVEUNIVERSALJOINT), joint(j), speed1(s1), speed2(s2)
    {}
  };
      
  /// Beam
  struct BeamAction : public RC3DAction
  {
    /// The position to beam to
    Eigen::Vector3d pos;
        
    BeamAction(Eigen::Vector3d const& p)
      : RC3DAction(BEAM), pos(p)
    {}
  };

  /// Say
  struct SayAction : public RC3DAction
  {
    std::string message;
    
    SayAction(std::string const& m)
      : RC3DAction(SAY), message(m)
    {}
  };
}
#endif // __BATS_RC3DACTION_HH_
