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

#ifndef _BATS_STATEVARNODE_HH_
#define _BATS_STATEVARNODE_HH_

#include "../ast.hh"
#include "../../Range/range.hh"

namespace bats
{
  typedef std::pair<std::string, Range> StateVar;
  typedef std::pair<std::string, std::string> StateVarStr;
  
  class StateVarNode : public AST::Node
  {
      StateVar d_var;
      StateVarStr d_varStr;
    public:
      StateVarNode(StateVar const& var) : Node(varType), d_var(var) {}
      
      /**
        *  Create a StateVarNode with StateVar(name,Range(r0,r1)).
        */
      StateVarNode(std::string const& name, double r0, double r1) : Node(varType), d_var(StateVar(name, Range(r0, r1))) {}
      
      StateVarNode(std::string const& name, std::string const& v) : Node(varType), d_varStr(StateVarStr(name, v)) {}
      
      bool match(std::string const &query) const { return d_var.first == query || d_varStr.first == query; }
      
      /**
        * @returns the value of the state variable
        */
      StateVar getVar() { return d_var; }
      StateVarStr getVarStr() { return d_varStr; }
  };
}

#endif

