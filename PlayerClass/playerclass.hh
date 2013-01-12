#ifndef __BATS_PLAYERCLASS_HH_
#define __BATS_PLAYERCLASS_HH_

#include "../Conf/conf.hh"
#include <map>
#include <memory>

namespace bats
{
  /** Player class
   *
   * Base class for implementing different classes that players can belong to.
   */
  class PlayerClass
  {
  public:
    /// Maps an index to a player class
    typedef std::map<unsigned, std::shared_ptr<PlayerClass> > PCMap;
    
    PlayerClass(unsigned idx);
    
    /// Creates playerclasses from configuration file
    template<typename T>
    static void createPlayerClasses();
    
    /// Get all created playerclasses
    static PCMap getPlayerClasses() { return s_playerClasses; }
    
    /// Get the player class with the given index
    static std::shared_ptr<PlayerClass> getPlayerClass(unsigned idx) { return s_playerClasses[idx]; }

  protected:
    unsigned d_idx;
    
    static PCMap s_playerClasses;
  };
  

  // Member implementations
  inline PlayerClass::PlayerClass(unsigned idx)
    : d_idx(idx)
  {
  }

  template<typename T>
  void PlayerClass::createPlayerClasses()
  {
    Conf& conf = SConf::getInstance();
    
    XMLNodeSet PCNS = conf.selectXPath("/conf/player-class");
    for (XMLNodeSet::iterator iter = PCNS.begin(); iter != PCNS.end(); ++iter)
    {
      XMLNode PCNode = *iter;
      if (!PCNode.hasProp("index"))
      {
        continue;
      }
      
      unsigned idx = atoi(PCNode.getProp("index").c_str());
      std::shared_ptr<PlayerClass> pc = s_playerClasses[idx] = std::make_shared<T>(idx);
    }
  }
  

}

#endif
