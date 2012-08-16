#ifndef __BATS_EMPTYDEBUGGER_HH__
#define __BATS_EMPTYDEBUGGER_HH__

#include "../debugger.hh"

namespace bats
{
  class EmptyDebugger : public Debugger
  {
      friend class bats::Singleton<Debugger>;

    public:
      virtual void init(int argc, char** argv) {}
      virtual void start() {}
      virtual void run() {}
      
      virtual void reDraw() {}
      
      virtual void debug(std::string const& dbg) {}
      
      virtual void plot(std::string const& curve, float value) {}

      virtual void draw(std::shared_ptr<Shape> shape) {}
      virtual void drawForPeriod(std::shared_ptr<Shape> shape, double durationSeconds) {}
      virtual void drawUntil(std::shared_ptr<Shape> shape, double time) {}

      virtual bool isEnabled() { return false; }
      virtual bool isVerbose() { return false; }
      virtual void setVerbose(bool isVerbose) { }
      
    private:
  };

};


#endif /* __BATS_EMPTYDEBUGGER_HH__ */

