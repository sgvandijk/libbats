#ifndef BATS_SITUATIONDETECTOR_HH
#define BATS_SITUATIONDETECTOR_HH

#include "../Singleton/singleton.hh"

#include <string>

namespace bats
{
  /** Situation detector base class
   *
   * This class provides a basic interface to keep track of the
   * current game situation. This can be used to define different
   * configurations and behavior during different situations.
   */
  class SituationDetector
  {
  public:
    virtual ~SituationDetector() {}

    /** Get the current game situation */
    virtual std::string getSituation() = 0;
  };

  typedef Singleton<SituationDetector> SSituationDetector;
}

#endif
