 /*
 *  Little Green BATS (2008), AI department, University of Groningen
 *
 *  Authors:         Martin Klomp (martin@ai.rug.nl)
 *                Mart van de Sanden (vdsanden@ai.rug.nl)
 *                Sander van Dijk (sgdijk@ai.rug.nl)
 *                A. Bram Neijt (bneijt@gmail.com)
 *                Matthijs Platje (mplatje@gmail.com)
 *
 *        All students of AI at the University of Groningen
 *  at the time of writing. See: http://www.ai.rug.nl/
 *
 *  Date:         November 1, 2008
 *
 *  Website:        http://www.littlegreenbats.nl
 *
 *  Comment:        Please feel free to contact us if you have any 
 *                problems or questions about the code.
 *
 *
 *  License:         This program is free software; you can redistribute 
 *                it and/or modify it under the terms of the GNU General
 *                Public License as published by the Free Software 
 *                Foundation; either version 3 of the License, or (at 
 *                your option) any later version.
 *
 *                   This program is distributed in the hope that it will
 *                be useful, but WITHOUT ANY WARRANTY; without even the
 *                implied warranty of MERCHANTABILITY or FITNESS FOR A
 *                PARTICULAR PURPOSE.  See the GNU General Public
 *                License for more details.
 *
 *                   You should have received a copy of the GNU General
 *                Public License along with this program; if not, write
 *                to the Free Software Foundation, Inc., 59 Temple Place - 
 *                Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef __INC_BATS_XMLATTR_HH_
#define __INC_BATS_XMLATTR_HH_

#include <libxml/tree.h>

#include <string>
#include <vector>

namespace bats {

  /**
   *  A XML node.
   \\TODO fix the memory leaks with xmlFree
   */
  class XMLAttr {
    xmlAttr *d_attr;

    void destroy()
    {
    }

    void copy(XMLAttr const &other)
    {
      d_attr = other.d_attr;
    }

  public:

    class Iterator {
      friend class XMLAttr;
      friend class XMLNode;

      xmlAttr *d_attr;

      Iterator(xmlAttr *attr)
        : d_attr(attr)
      {
      }

    public:

      Iterator &operator++()
      {
        d_attr = d_attr->next;
        return *this;
      }

      Iterator &operator--()
      {
        d_attr = d_attr->prev;
        return *this;
      }

      XMLAttr operator*()
      {
        return XMLAttr(d_attr);
      }

      bool operator==(Iterator const &other)
      {
        return d_attr == other.d_attr;
      }

      bool operator!=(Iterator const &other)
      {
        return d_attr != other.d_attr;
      }

    };

    typedef Iterator iterator;

    XMLAttr(XMLAttr const &other) { copy(other); }

    XMLAttr(xmlAttr *node)
      : d_attr(node)
    {}

    ~XMLAttr() { destroy(); }

    XMLAttr &operator=(XMLAttr const &other)
    {
      if (this != &other) {
        destroy();
        copy(other);
      }
      return *this;
    }

    bool operator==(XMLAttr const &other)
    {
      return d_attr == other.d_attr;
    }

    /**
     *  @returns the next node.
     */
    XMLAttr getNext() { return XMLAttr(d_attr->next); }

    /**
     *  @returns the name/tagname of the node.
     */
    std::string getName() { return reinterpret_cast<char const*>(d_attr->name); }

    /**
     *  @returns the value of the attribute (I guess).
     */
    std::string getValue() const
    {
      return reinterpret_cast<char*>(xmlNodeGetContent(d_attr->children));
    }

    /**
     *  @returns false if the node is NULL.
     */
    operator bool() const { return d_attr; }

  };

};

#endif // __INC_BATS_XMLATTR_HH_
