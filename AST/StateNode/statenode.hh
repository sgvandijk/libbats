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

#ifndef _BATS_STATENODE_HH_
#define _BATS_STATENODE_HH_

#include "../ast.hh"
#include "../OrNode/ornode.hh"

namespace bats
{
  /** A State Node
    *
    * Root node of a full state tree. Children should be disjuncts, grandchildren conjuncts and grandgrandchildren state variable nodes
    */
  class StateNode : public AST::Node {

  public:

    StateNode(unsigned type = 0) : Node(type) {}

    /**
      *  @returns a new disjunction which has been appended to the children of this node
      */
    std::shared_ptr<OrNode> addDisjunct()
    {
      return std::static_pointer_cast<OrNode>(addChild(std::make_shared<OrNode>()));
    }

    /** 
      *  @returns a new disjunction which has been appended to the children of this node, containing a new conjunction
      */
    std::shared_ptr<AndNode> addBoth()
    {
      std::shared_ptr<OrNode> dis(std::static_pointer_cast<OrNode>(addChild(std::make_shared<OrNode>())));
      return std::static_pointer_cast<AndNode>(dis->addConjunct());
    }

    /**
      * @returns a vector containing the values of the state variables nameX, nameY and nameZ
      */
    Eigen::Vector3d getVector(std::string const &name) const
    {
      std::shared_ptr<StateVarNode> X = std::static_pointer_cast<StateVarNode>(findDeep(name + "X"));
      std::shared_ptr<StateVarNode> Y = std::static_pointer_cast<StateVarNode>(findDeep(name + "Y"));
      std::shared_ptr<StateVarNode> Z = std::static_pointer_cast<StateVarNode>(findDeep(name + "Z"));

      if (!X || !Y || !Z)
        throw std::runtime_error("Vector var node '" + name + "' not found in Goal");

      StateVar x = X->getVar();
      StateVar y = Y->getVar();
      StateVar z = Z->getVar();

      return Eigen::Vector3d(x.second.mean(), y.second.mean(), z.second.mean());
    }
    
    /**
      * @returns the mean of the state variable @name
      */
    double getMean(std::string const &name) const
    {
      std::shared_ptr<StateVarNode> node = std::static_pointer_cast<StateVarNode>(findDeep(name));     	
      assert(node);
      StateVar x = node->getVar();
      return x.second.mean();
    }
  };

  /// A state is a conjunction of state variables, implemented as a tree
  typedef StateNode State;

  /// A goal is a state
  typedef State Goal;
}

#endif
