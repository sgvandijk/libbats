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
#ifndef INC_BATS_CONF_HH
#define INC_BATS_CONF_HH

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdexcept>
#include <cstdlib>
#include <sstream>

#include "xmlnode.hh"
#include "xmlnodeset.hh"

#include "../Singleton/singleton.hh"

namespace bats {

  /** Conf class
   *
   *  In this class the XML configuration file is parsed. It also provides
   *  some functionality for retrieving parts of the config tree. It's up
   *  to the using classes to retrieve the relevant parts for that class.
   *
   *  This is a Singleton, use initialize() before using.
   */
  class Conf {
  public:

    ~Conf() { destroy(); }

    /**
     * Set the name of the file that Conf should load.
     */
    void parseFile(std::string const &confFile);

    /**
     *  @returns the absolute root node of the config tree.
     */
    XMLNode getRoot();

    /**
     *  @returns a set of nodes which match the XPATH expression expr.
     *  @see http://www.w3schools.com/xpath/
     */
    XMLNodeSet selectXPath(std::string const &expr) const;

    /**
     *  @returns the value of the value attribute of the node specified with
     *           the xpath of /conf/parameters/name. If the parameter does not
     *           exist the value def is returned.
     */
    template <typename T>
    T getParam(std::string const &name, T def) const;
    
    /**
     *  @returns the value of the value attribute of the node
     *           specified with the xpath of
     *           /conf/player-class[\@idx=playerClassIdx]/parameters/name. If
     *           the parameter does not exist the value def is
     *           returned.
     */
    template <typename T>
    T getParam(unsigned playerClassIdx, std::string const &name, T def) const;

  private:
    friend class Singleton<Conf>;
    
    xmlDocPtr d_doc;
    xmlXPathContextPtr xpathCtx;

    void defineEntities(xmlDocPtr doc);

    void parseConf(std::string const &confFile);

    void destroy()
    {
      if (xpathCtx)
        xmlXPathFreeContext(xpathCtx); 

      if (d_doc)
        xmlFreeDoc(d_doc);

      xmlCleanupParser();
    }

    XMLNode getParam(std::string const &name);
    
    Conf();
    
    /**
     *  @param confFile specifies the path to the xml configuration file.
     */
    Conf(std::string const &confFile);

  };

  // Member implementations

  inline void Conf::parseFile(std::string const &confFile)
  {
    parseConf(confFile);
    
    if (xpathCtx)
      xmlXPathFreeContext(xpathCtx); 
    xpathCtx = xmlXPathNewContext(d_doc);
  }

  inline XMLNode Conf::getRoot()
  {
    return xmlDocGetRootElement(d_doc);
  }

  template <typename T>
  T Conf::getParam(std::string const &name, T def) const
  {
    XMLNodeSet ns = selectXPath(std::string("/conf/parameters/") + name);
    
    if(ns && !ns.empty())
    {
      std::istringstream s(ns.front().getContent());
      s >> def;
    }
    return def;
  };

  template <typename T>
  T Conf::getParam(unsigned playerClassIdx, std::string const &name, T def) const
  {
    std::ostringstream xpath("");
    xpath << "/conf/player-class[@idx=" << playerClassIdx << "]/parameters/" << name;
    XMLNodeSet ns = selectXPath(xpath.str());
    if(ns && !ns.empty())
    {
      std::istringstream s(ns.front().getContent());
      s >> def;
    }
    return def;
  } 

  typedef Singleton<Conf> SConf;
};

#endif // __INC_BATS_CONF_HH_
