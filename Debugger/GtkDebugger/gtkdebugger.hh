/*
 *  Little Green BATS (2008-2013), AI department, University of Groningen
 *
 *  Authors: 	Sander van Dijk (sgdijk@ai.rug.nl)
 *		Drew Noakes (drew@drewnoakes.com)
 *		Martin Klomp (martin@ai.rug.nl)
 *		Mart van de Sanden (vdsanden@ai.rug.nl)
 *		A. Bram Neijt (bneijt@gmail.com)
 *		Matthijs Platje (mplatje@gmail.com)
 *		Jeroen Kuijpers (jkuypers@gmail.com)
 *
 *  Date: 	August 17, 20013
 *
 *  Website:	https://github.com/sgvandijk/libbats
 *
 *  Comment:	Please feel free to contact us if you have any 
 *		problems or questions about the code.
 *
 *
 *  License: 	This program is free software; you can redistribute 
 *		it and/or modify it under the terms of the GNU General
 *		Public License as published by the Free Software 
 *		Foundation; either version 3 of the License, or (at 
 *		your option) any later version.
 *
 *   		This program is distributed in the hope that it will
 *		be useful, but WITHOUT ANY WARRANTY; without even the
 *		implied warranty of MERCHANTABILITY or FITNESS FOR A
 *		PARTICULAR PURPOSE.  See the GNU General Public
 *		License for more details.
 *
 *   		You should have received a copy of the GNU General
 *		Public License along with this program; if not, write
 *		to the Free Software Foundation, Inc., 59 Temple Place - 
 *		Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef BATS_GTKDEBUGGER_HH
#define BATS_GTKDEBUGGER_HH

#include "../debugger.hh"

#include <gtkmm.h>
#include <queue>

namespace bats
{
  extern "C" void onDrawingAreaExpose(GtkObject *object, gpointer user_data);

  /** GTK debugger implementation
   *
   * @ingroup debugging
   */
  class GtkDebugger : public Debugger
  {
    friend class Singleton<Debugger>;
    
    public:
      void init(int argc, char** argv);
      void start();
      void run();
      
      void reDraw();
      
      void debug(std::string const& dbg)
      {
        if (d_running)
        {
          d_dbgTextLock.lock();
          d_dbg += dbg;
          d_dbgTextLock.unlock();
        }
      }

      bool isEnabled() { return true; }
      bool isVerbose() { return false; }
      void setVerbose(bool isVerbose) { }
      
      void plot(std::string const& curve, float value);
      
      void draw(std::shared_ptr<Shape> shape) { d_shapes.push_back(shape); }
      void drawForPeriod(std::shared_ptr<Shape> shape, double durationSeconds) {}
      void drawUntil(std::shared_ptr<Shape> shape, double time) {}
      
      Glib::Dispatcher redraw_dispatch;
      Glib::Dispatcher dbg_text_dispatch;
      
    private:
      bool d_running;
      unsigned d_ticks;
      
      pthread_t d_thread;
      Gtk::Main* d_kit;
      
      Gtk::Window* d_window;
      Gtk::DrawingArea* d_drawingArea;
      Gtk::TextView* d_dbgText;
      Gtk::DrawingArea* d_curveDrawingArea;
      Gtk::TreeView* d_curvesList;
      Glib::RefPtr<Gtk::ListStore> d_curvesListStore;
      Glib::RefPtr<Gtk::TreeStore> d_behaviorTreeStore;
      
      Glib::Mutex d_dbgTextLock;
      std::string d_dbg;
      
      std::map<std::string, Curve> d_curves;
      
      std::vector<std::shared_ptr<Shape> > d_shapes;
      
      GtkDebugger ();

      void onRedraw() { d_drawingArea->queue_draw(); }
      void onThinkEnd();
      void onDbgText();

      void drawField(Cairo::RefPtr<Cairo::Context> context);
      void drawSelf(Cairo::RefPtr<Cairo::Context> context);
      void drawBall(Cairo::RefPtr<Cairo::Context> context);
      void drawPlayers(Cairo::RefPtr<Cairo::Context> context);
      void drawShapes(Cairo::RefPtr<Cairo::Context> context);
      
      void drawCurve();
  };
  
};


#endif /* BATS_GTKDEBUGGER_HH */

