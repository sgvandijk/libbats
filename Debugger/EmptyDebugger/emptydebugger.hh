#ifndef BATS_EMPTYDEBUGGER_HH
#define BATS_EMPTYDEBUGGER_HH

#include "../debugger.hh"

namespace bats
{
  /** Dummy debugger
   *
   * You can initialize the Debugger singleton with this class to
   * disable all debugging.
   */
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


#endif /* BATS_EMPTYDEBUGGER_HH */

