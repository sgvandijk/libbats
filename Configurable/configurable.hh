#ifndef BATS_CONFIGURABLE_HH
#define BATS_CONFIGURABLE_HH

#include <sstream>

namespace bats
{
  /** Base class template for everything that can be configured using XML
   */
  class Configurable
  {
  public:
    Configurable(std::string const& tag, std::string const& id);

    std::string getTag() const;
    std::string getID() const;

  protected:
    /** Set the tag used in the configuration for this configurable
     */
    void setTag(std::string const& tag);

    /** Get a parameter of a specific type from the XML configuration
     * 
     * This function will look for an element (directly
     * under the "conf" root node) with the tag and id of the instance it is
     * called on. Example XML:
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
     * @param def Default value when the parameter is not defined in
     * the configuration
     * @returns the node of the requested parameter or @a def when the
     * parameter isn't found
     */
    template <typename T> T getConfParam(std::string const &xpath, T def) const;

  private:
    std::string d_tag;
    std::string d_id;
  };



  /*
   * Inline methods
   */
  std::string Configurable::getTag() const
  {
    return d_tag;
  }

  std::string Configurable::getID() const
  {
    return d_id;
  }

  void Configurable::setTag(std::string const& tag)
  {
    d_tag = tag;
  }

  template <typename T>
  T Configurable::getConfParam(std::string const &xpath, T def) const
  {
    std::string fullPath = std::string("/conf/" + d_tag + std::string("['@id=") + d_id + std::string("']") + xpath;
    bats::XMLNodeSet ns(bats::SConf::getInstance().selectXPath(fullPath));
    if(ns && !ns.empty())
    {
      std::istringstream s(ns.front().getContent());
      s >> def;
    }
    return def;
  };

}

#endif
