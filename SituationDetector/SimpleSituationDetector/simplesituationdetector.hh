#ifndef BATS_SIMPLESITUATIONDETECTOR_HH
#define BATS_SIMPLESITUATIONDETECTOR_HH

#include "../situationdetector.hh"
#include "../../WorldModel/worldmodel.hh"

namespace bats
{
  /** Simple playmode situation detector iplementation
   *
   * This implementation simply returns the current play maode as a
   * string as the current situation.
   */
  class SimpleSituationDetector : public SituationDetector
  {
  public:
    friend class Singleton<SituationDetector>;

    virtual std::string getSituation() {
      return Types::nameOf(SWorldModel::getInstance().getPlayMode());
    }

  private:
    SimpleSituationDetector() {}
    SimpleSituationDetector(SimpleSituationDetector const&); //NI
    SimpleSituationDetector& operator=(SimpleSituationDetector const&); //NI
  };
}

#endif
