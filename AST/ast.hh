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
#ifndef INC_BATS_AST_HH
#define INC_BATS_AST_HH

#include <memory>
#include <vector>
#include <Eigen/Core>
#include "../Path/path.hh"

namespace bats {

  
  /** Abstract Syntax Tree.
   */
  namespace AST {
    
    /** Tree Node base class
     *
     * Inherit from this node to create specific tree nodes.
     */
    class Node {

      //Node(Node const &); // NI
      //Node &operator=(Node const &); //

    public:

      /// A vector of Nodes
      typedef std::vector< std::shared_ptr<Node> > NodeVector;

      typedef NodeVector::const_iterator const_iterator;
      typedef NodeVector::iterator iterator;

    protected:

      unsigned d_type;
      NodeVector d_nodes;
      
    public:

      ///@name Construction
      ///@{

      /**
       *  @param type is the node type, zero means NONO or ERROR.
       */
      Node(unsigned type = 0)
        : d_type(type) {}

      virtual ~Node() {}

      /**
       *  Adds a child node to this node.
       */
      std::shared_ptr<Node> addChild(std::shared_ptr<Node> child) { d_nodes.push_back(child); return child; }


      ///@}

      ///@name Properties
      ///@{

      /**
       *  @returns this node's type.
       */
      unsigned getType() const { return d_type; }

      /**
       *  @returns false if this node has no type.
       */
      virtual bool isOk() const { return d_type; }

      /**
       *  @returns false if and only if this node's type is zero.
       */
      operator bool() const { return isOk(); }

      /**
       *  @returns true if this is not a leaf node.
       */
      bool hasChildren()
      {
        return d_nodes.size() > 0;
      }

      /**
       *  @returns true if this is a leaf node (has no children).
       */
      bool isLeaf() const { return d_nodes.empty(); }

      /**
       *  @returns the number of children.
       */
      unsigned size() const { return d_nodes.size(); }

      ///@}

      ///@name Traversal
      ///@{

      /**
       *  @returns a const vector of children.
       */
      NodeVector const &getChildren() const { return d_nodes; }

      /**
       *  @returns an iterator pointing at the first child.
       */
      iterator begin() { return d_nodes.begin(); }

      /**
       *  @returns an iterator pointing past the last child.
       */
      iterator end() { return d_nodes.end(); }

      /**
       *  @returns a const iterator pointing at the first child.
       */
      const_iterator begin() const { return d_nodes.begin(); }

      /**
       *  @returns a const iterator pointing past the last child.
       */
      const_iterator end() const { return d_nodes.end(); }

      /**
       *  @returns the child at index.
       */
      std::shared_ptr<Node> getChild(unsigned index)
      {
        assert(index < d_nodes.size());
        return d_nodes[index];
      }
      
      /**
       *  @returns the child at index.
       */
      std::shared_ptr<Node> const getChild(unsigned index) const
      {
        assert(index < d_nodes.size());
        return d_nodes[index];
      }

      /**
       *  @returns the child at index.
       */
      std::shared_ptr<Node> operator[](unsigned index) { return getChild(index); }

      ///@}

      ///@name Tree Search
      ///@{

      /**
       *  Used to match strings with nodes. Needs to be implemented
       *  for the find* methods to work.
       */
      virtual bool match(std::string const &query) const { return false; }

      /**
       *  Finds the first node witch matches query and returns it.
       *
       *  In most cases query will be the name or value of the node. The
       *  match() method is used to match the query to a node.
       */
      std::shared_ptr<Node> find(std::string const &query) const;

      /**
       *  Finds all nodes witch match query and appends them to res.
       *  @returns the number of matches.
       */
      unsigned findAll(std::vector<std::shared_ptr<Node> > &res, std::string const &query) const;
      
      /**
       *  Finds the first node in the entire tree witch matches query and returns it, using a depth-first search.
       *
       *  Warning: If there is a loop in the tree, this function will
       *           never exit! (So we need loop detection)
       *
       */
      std::shared_ptr<Node> findDeep(std::string const &query) const;

			Eigen::Vector3d getVector(std::string const &name) const;
			
      /**
       *  Finds all nodes in the entire tree witch match query and appends them to res.
       *
       *  Warning: If there is a loop in the tree, this function will
       *           never exit! (So we need loop detection)
       *
       *  @returns the number of hits.
       */
      unsigned findAllDeep(std::vector<std::shared_ptr<Node> > &res, std::string const &query) const;

      /**
       *  Finds all the predicates that comply with the specified select string
       *  and pushes them into res.
       *  @return ths number of hits.
       */
      unsigned selectAll(std::vector<std::shared_ptr<Node> > &res, Path const &select) const;

      /**
       *  @returns the first predicate that complies with select.
       */
      std::shared_ptr<Node> select(Path const &select) const;

      ///@}

    };
    
  };
  
};


#endif // INC_BATS_AST_HH
