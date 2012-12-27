#ifndef BATS_CONFIGURABLE_HH
#define BATS_CONFIGURABLE_HH

#include <sstream>
#include "../Conf/conf.hh"

namespace bats
{
  /** Base class template for everything that can be configured using XML
   */
  class Configurable
  {
  public:
    Configurable(std::string const& tag, std::string const& id)
      : d_tag(tag),
	d_id(id)
    {}

    std::string getTag() const;
    std::string getID() const;

  protected:
    /** Set the tag used in the configuration for this configurable
     */
    void setTag(std::string const& tag);

    /** Get the content of a parameter from the XML configuration
     *
     * This function will look for an element (directly under the
     * "conf" root node) with the tag and id of the instance it is
     * called on. For example, in the following example the two
     * parameters can be read using xpaths "/param1" and "/param2"
     * respectively:
     * 
     * <conf>
     *   ...
     *   <mytagname id="myconfigurable">
     *     <param1>value1</param1>
     *     <param2>value2</param2>
     *   </mytagname>
     *   ...
     * </conf>
     *
     * where 'mytagname' and 'myconfigurable' are the tag and id given
     * on construction.
     *
     * @param xpath The xpath of the parameter relative to this
     * configurable's node
     *
     * @returns the content of the requested parameter as a string, or
     * an empty string when the parameter isn't found
     */
    std::string getConfParamContent(std::string const& &xpath) const;

    /** Get a parameter of a specific type from the XML configuration
     * 
     * This function will look for an element (directly under the
     * "conf" root node) with the tag and id of the instance it is
     * called on, the same as @getConfParamContent, and return the
     * content parsed into the desired type.
     *
     * @param xpath The xpath of the parameter relative to this
     * configurable's node @param def Default value when the parameter
     * is not defined in the configuration; the type of this value
     * also determins the type of the return value.
     *
     * @returns the node of the requested parameter or @a def when the
     * parameter isn't found
     */
    template <typename T>
    T getConfParam(std::string const &xpath, T def) const;

  private:
    std::string d_tag;
    std::string d_id;
  };


  /*******************************
   * 
   * Inline method implementations
   * 
   *******************************/

  inline std::string Configurable::getTag() const  { return d_tag; }

  inline std::string Configurable::getID() const { return d_id; }

  inline void Configurable::setTag(std::string const& tag) { d_tag = tag; }

  inline Configurable::getConfParamContent(std::string const& xpath) const
  {
    std::string fullPath = std::string("/conf/") + d_tag + std::string("['@id=") + d_id + std::string("']") + xpath;
    bats::XMLNodeSet ns(bats::SConf::getInstance().selectXPath(fullPath));
    if(ns && !ns.empty())
    {
      return ns.front().getContent();
    }
    return "";
  }

  template <typename T>
  T Configurable::getConfParam(std::string const &xpath, T def) const
  {
    std::string fullPath = std::string("/conf/") + d_tag + std::string("['@id=") + d_id + std::string("']") + xpath;
    bats::XMLNodeSet ns(bats::SConf::getInstance().selectXPath(fullPath));
    if(ns && !ns.empty())
    {
      std::istringstream s(ns.front().getContent());
      s >> def;
    }
    return def;
  }

}

#endif
