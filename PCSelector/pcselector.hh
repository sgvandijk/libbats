#ifndef BATS_PCSELECTOR_HH
#define BATS_PCSELECTOR_HH

#include "../Singleton/singleton.hh"

namespace bats
{
  /** Player class selector base class
   *
   * This class provides a basic interface to keep track of the player
   * class of the current agent. This can be used to define different
   * configurations and behavior for different agents.
   */
  class PCSelector
  {
  public:
    virtual ~PCSelector() {}

    /** Get the player class of the current agent
     *
     * This may change over time, based on the chosen player class
     * selector implementation.
    */
    virtual unsigned getPlayerClass() = 0;
    

    /** Set the player class of the current agent
     *
     * This may be ignored, based on the chosen player class selector
     * implementation.
    */
    virtual void setPlayerClass(unsigned c) {}
  };

  typedef Singleton<PCSelector> SPCSelector;
}

#endif
