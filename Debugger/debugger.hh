#ifndef __BATS_DEBUGGER_HH__
#define __BATS_DEBUGGER_HH__

#include "../Singleton/singleton.hh"
#include "../BatsEvent/batsevent.hh"

#include "../Shape/shape.hh"

#include <queue>
#include <sstream>

namespace bats
{
  class Debugger
  {
  public:
    struct Curve
    {
      Curve() : minX(0), maxX(0), minY(0), maxY(0) {}
      
      std::vector<float> data;
      float minX;
      float maxX;
      float minY;
      float maxY;
    };
    
    virtual ~Debugger() {}

    virtual void init(int argc, char** argv) = 0;
    virtual void start() = 0;
    
    virtual void reDraw() = 0;
    
    virtual void debug(std::string const& dbg) = 0;
    
    virtual void plot(std::string const& curve, float value) = 0;
    
    virtual void draw(std::shared_ptr<Shape> shape) = 0;
    virtual void drawForPeriod(std::shared_ptr<Shape> shape, double durationSeconds) = 0;      
    virtual void drawUntil(std::shared_ptr<Shape> shape, double time) = 0; 
    
    /** Indicates whether debugging is enabled.  If this value is false, then calls to other methods on this type have no effect. */
    virtual bool isEnabled() = 0;
    
    /** Get whether we're showing all debug output, or only a selection (based on the player in possession, for example.) */
    virtual bool isVerbose() = 0;
    
    /** Set whether we're showing all debug output, or only a selection (based on the player in possession, for example.) */
    virtual void setVerbose(bool isVerbose) = 0;
  };
  
  template<typename T>
  Debugger& operator<<(Debugger& debugger, T const& dbg)
  {
    std::ostringstream out;
    out << dbg;
    debugger.debug(out.str());
    return debugger;
  }
  
  typedef Singleton<Debugger> SDebugger;
};


#endif /* __BATS_DEBUGGER_HH__ */

