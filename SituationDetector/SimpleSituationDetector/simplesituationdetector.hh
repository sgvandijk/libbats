#ifndef __BATS_SIMPLESITUATIONDETECTOR_HH_
#define __BATS_SIMPLESITUATIONDETECTOR_HH_

#include "../situationdetector.hh"
#include "../../WorldModel/worldmodel.hh"

namespace bats
{
  class SimpleSituationDetector : public SituationDetector
  {
    SimpleSituationDetector() {}
    SimpleSituationDetector(SimpleSituationDetector const&); //NI
    SimpleSituationDetector& operator=(SimpleSituationDetector const&); //NI
    
  public:
    friend class Singleton<SituationDetector>;

    virtual std::string getSituation() { return Types::nameOf(SWorldModel::getInstance().getPlayMode()); }
  };
}

#endif
