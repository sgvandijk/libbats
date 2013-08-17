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
#ifndef __INC_BATS_XMLNODE_HH_
#define __INC_BATS_XMLNODE_HH_

#include <libxml/tree.h>

#include <string>
#include <vector>

#include "xmlattr.hh"

#include "../Path/path.hh"

namespace bats {

  /**
   *  A XML node.
   \\todo fix the memory leaks with xmlFree
   */
  class XMLNode {
    xmlNode *d_node;

    void destroy()
    {
    }

    void copy(XMLNode const &other)
    {
      d_node = other.d_node;
    }

  public:

    class Iterator {
      friend class XMLNode;

      xmlNode *d_node;

      Iterator(xmlNode *node)
        : d_node(node)
      {
      }

    public:

      Iterator &operator++()
      {
        d_node = d_node->next;
        return *this;
      }

      Iterator &operator--()
      {
        d_node = d_node->prev;
        return *this;
      }

      XMLNode operator*()
      {
        return XMLNode(d_node);
      }

      bool operator==(Iterator const &other)
      {
        return d_node == other.d_node;
      }

      bool operator!=(Iterator const &other)
      {
        return d_node != other.d_node;
      }

    };

    typedef Iterator iterator;

    XMLNode(XMLNode const &other) { copy(other); }

    XMLNode(xmlNode *node)
      : d_node(node)
    {}

    ~XMLNode() { destroy(); }

    XMLNode &operator=(XMLNode const &other)
    {
      if (this != &other) {
        destroy();
        copy(other);
      }
      return *this;
    }

    bool operator==(XMLNode const &other) const
    {
      return d_node == other.d_node;
    }

    /**
     *  @returns the next node.
     */
    XMLNode getNext() { return XMLNode(d_node->next); }

    /**
     *  @returns the name/tagname of the node.
     */
    std::string getName() const { return reinterpret_cast<char const*>(d_node->name); }

    /**
     *  @returns the child node.
     */
    XMLNode getChildren() const { return XMLNode(d_node->children); }

    /**
     *  @returns true if property/attribute @name is defined.
     */
    bool hasProp(std::string const &name) const
    { return xmlHasProp(d_node,reinterpret_cast<xmlChar const*>(name.c_str())); }

    /**
     *  @returns the value of the property with name @name.
     */
    std::string getProp(std::string const &name) const
    {
      xmlChar *xprop = xmlGetProp(d_node,reinterpret_cast<xmlChar const *>(name.c_str()));
      if (!xprop)
        return "";
      std::string prop(reinterpret_cast<char*>(xprop));
      xmlFree(xprop);
      return prop;
    }

    /**
     *  @returns the content of the node.
     */
    std::string getContent() const
    {
      xmlChar* c = xmlNodeGetContent(d_node);
      std::string content(reinterpret_cast<char*>(c));
      xmlFree(c);
      return content;
    }

    /**
     *  @returns false if the node is NULL.
     */
    operator bool() const { return d_node; }

    /**
     *  Replace this node with another node
     */
    void replaceWith(XMLNode const& other) { xmlReplaceNode(d_node, other.d_node); }
    
    /**
     *  Finds the first node with @name and returns it.
     */
    XMLNode find(std::string const &name) const
    {
      for (XMLNode i = getChildren(); i; i = i.getNext())
        if (i.getName() == name)
          return i;
      return 0;
    }

    /**
     *  Finds all nodes localy with @name and pushes them into @res.
     */
    unsigned findAll(std::vector<XMLNode> &res, std::string const &name) const
    {
      unsigned c = 0;
      for (XMLNode i = getChildren(); i; i = i.getNext())
        if (i.getName() == name) {
          res.push_back(i);
          ++c;
      }
      return c;
    }

    /**
     *  Finds all nodes tree wide with @name ande pushes them into @res.
     */
    unsigned findAllDeep(std::vector<XMLNode> &res, std::string const &name) const
    {
      unsigned cnt = 0;

      for (XMLNode i = getChildren(); i; i = i.getNext()) {
        cnt += i.findAllDeep(res,name);

        if (i.getName() == name)
        {
          res.push_back(i);
          ++cnt;
        }
      }

      return cnt;
    }

    /**
     *  Finds the first node (tree wide) that has @name.
     */
    XMLNode findDeep(std::string const &name) const
    {
      for (XMLNode i = getChildren(); i; i = i.getNext()) {
        if (i.getName() == name)
          return i;
        else {
          XMLNode p = i.findDeep(name);
          if (p)
            return p;
        }
      }

      return 0;
    }

    unsigned selectAll(std::vector<XMLNode> &_res, Path const &_select) const
    {
      std::vector<XMLNode> search_space;

      Path p = _select;

      if (p.path.empty())
        return 0;

      if (p.path.front() == "/") {
        // Search in all predicates with value path[0] which are in
        // the top level of the tree.
        p.path.pop_front();
        if (p.path.empty()) {
          unsigned cnt = 0;
          for (XMLNode i = getChildren(); i; i = i.getNext()) {
            _res.push_back(i);
            ++cnt;
          }
          return cnt;
        } else
          findAll(search_space,p.path.front());
      } else {
        // Search in all predicates with value path[0].
        findAllDeep(search_space,p.path.front());
      }

      p.path.pop_front();

      unsigned cnt = 0;

      for (std::vector<XMLNode>::iterator i = search_space.begin();
           i != search_space.end(); ++i) {
        if (p.path.empty()) {
          _res.push_back(*i);
          ++cnt;
        } else
          cnt += i->selectAll(_res,p);
      }
      
      return cnt;
    }

    XMLNode select(Path const &_select) const
    {
      std::vector<XMLNode> lst;

      // This is a hack, I will improve it later.
      if (selectAll(lst,_select))
        return lst.front();
      else
        return 0;
    }

    iterator begin()
    {
      return Iterator(d_node->children);
    }

    iterator end()
    {
      return Iterator(0);
    }

    XMLAttr::iterator beginAttr()
    {
      return XMLAttr::iterator(d_node->properties);
    }

    XMLAttr::iterator endAttr()
    {
      return XMLAttr::iterator(0);
    }

  };

};

#endif // __INC_BATS_XMLNODE_HH_
