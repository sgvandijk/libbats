#ifndef __BATS_PLAYERCLASS_HH_
#define __BATS_PLAYERCLASS_HH_

#include "../Conf/conf.hh"
#include <map>
#include <memory>

namespace bats
{
  class PlayerClass
  {
  public:
    typedef std::map<unsigned, std::shared_ptr<PlayerClass> > PCMap;
    
    PlayerClass(unsigned idx)
    : d_idx(idx)
    {
    }
    
    template<typename T>
    static void createPlayerClasses();
    
    static PCMap getPlayerClasses() { return s_playerClasses; }
    
    static std::shared_ptr<PlayerClass> getPlayerClass(unsigned idx) { return s_playerClasses[idx]; }

  protected:
    unsigned d_idx;
    
    static PCMap s_playerClasses;
  };
  
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
      std::shared_ptr<PlayerClass> pc = s_playerClasses[idx] = new T(idx);
    }
  }
  
}

#endif
