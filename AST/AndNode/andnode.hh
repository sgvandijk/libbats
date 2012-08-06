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

#ifndef _BATS_ANDNODE_HH_
#define _BATS_ANDNODE_HH_

#include "../ast.hh"
#include "../StateVarNode/statevarnode.hh"
#include "../../Distribution/distribution.hh"
#include "../../Math/basicmath.hh"

namespace bats
{
  class AndNode : public AST::Node {

  public:

    AndNode() : Node(andType) {}

    /**
      *  Create an AndNode with one state var with name and range(r0,r1).
      */
    AndNode(std::string const &name, double r0, double r1) : Node(andType)
    {
      addVar(name,r0,r1);
    }

    /**
      *  Add a state variable with name and range (r0,r1).
      */
    void addVar(std::string const &name, double r0, double r1)
    {
      addChild(new StateVarNode(name,r0,r1));
    }

    /**
     * Add a state variable with name and value
     */
    void addVar(std::string const &name, std::string const &val)
    {
      addChild(new StateVarNode(name, val));
    }
    
    /**
      * Add state variables based on a 3D distribution. The variables get the names nameX, nameY and nameZ. The range of a variable is set to \f$ (\mu - {1 over 2} \sqrt{s^2}, \mu + {1 over 2} \sqrt{s^2}) \f$
      */
    void addVar(std::string const &name, rf<Distribution> const &pos)
    {
      Eigen::Vector3d mu = pos->getMu();//.cast<Eigen::Vector3d>();
      Eigen::Matrix3d sigma = pos->getSigma();//.cast<Eigen::Matrix3d>();
      //  			pos->getSigma(sigma);
      double stdx2 = 2*BasicMath::sqrt(sigma(0,0));
      double stdy2 = 2*BasicMath::sqrt(sigma(1,1));
      double stdz2 = 2*BasicMath::sqrt(sigma(2,2));
      addVar(name+"X", mu(0) - stdx2, mu(0) + stdx2);
      addVar(name+"Y", mu(1) - stdy2, mu(1) + stdy2);
      addVar(name+"Z", mu(2) - stdz2, mu(2) + stdz2);
    }

    /**
      * Add state variables based on a 3D vector. The variables get the names nameX, nameY and nameZ.
      */
    void addVar(std::string const &name, Eigen::Vector3d const &pos)
    {
      addVar(name+"X", pos(0), pos(0));
      addVar(name+"Y", pos(1), pos(1));
      addVar(name+"Z", pos(2), pos(2));
    }

    /**
     * Add state variable based on two 3D vectors that determine the range. The variables get the names nameX, nameY and nameZ.
     */
    void addVar(std::string const &name, Eigen::Vector3d const& min, Eigen::Vector3d const& max)
    {
      addVar(name+"X", min(0), max(0));
      addVar(name+"Y", min(1), max(1));
      addVar(name+"Z", min(2), max(2));
    }

  };
}

#endif
