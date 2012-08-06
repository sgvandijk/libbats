#ifndef __BATS_PLAYERCLASS_HH_
#define __BATS_PLAYERCLASS_HH_

#include "../RefAble/refable.hh"
#include "../Ref/rf.hh"
#include "../Conf/conf.hh"
#include <map>

namespace bats
{
  class PlayerClass : public RefAble
  {
  public:
    typedef std::map<unsigned, rf<PlayerClass> > PCMap;
    
    PlayerClass(unsigned idx)
    : d_idx(idx)
    {
    }
    
    template<typename T>
    static void createPlayerClasses();
    
    static PCMap getPlayerClasses() { return s_playerClasses; }
    
    static rf<PlayerClass> getPlayerClass(unsigned idx) { return s_playerClasses[idx]; }

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
      rf<PlayerClass> pc = s_playerClasses[idx] = new T(idx);
    }
  }
  
}

#endif
