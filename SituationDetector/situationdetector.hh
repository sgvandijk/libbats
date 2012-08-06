#ifndef __BATS_SITUATIONDETECTOR_HH_
#define __BATS_SITUATIONDETECTOR_HH_

#include "../Singleton/singleton.hh"

#include <string>

namespace bats
{
  class SituationDetector
  {
  public:
    virtual ~SituationDetector() {}

    virtual std::string getSituation() = 0;
  };

  typedef Singleton<SituationDetector> SSituationDetector;
}

#endif
