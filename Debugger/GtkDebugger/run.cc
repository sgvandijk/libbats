#include "gtkdebugger.ih"
#include <iostream>

void GtkDebugger::run()
{
  d_kit = new Gtk::Main(0, 0);

  // Create GUI and gather widgets
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("Debugger/debugger.glade");
  gtk_builder_connect_signals(builder->gobj(), 0);
  
  builder->get_widget("debugWindow", d_window);
  
  builder->get_widget("drawingArea", d_drawingArea);
  
  builder->get_widget("dbgText", d_dbgText);
  
  d_curvesListStore = Glib::RefPtr<Gtk::ListStore>::cast_static(builder->get_object("curvesListStore"));
  builder->get_widget("curvesList", d_curvesList);
  builder->get_widget("curveDrawingArea", d_curveDrawingArea);
  
  d_behaviorTreeStore = Glib::RefPtr<Gtk::TreeStore>::cast_static(builder->get_object("behaviorTreeStore"));
 
  sigc::slot<void> sl = sigc::mem_fun(*this, &GtkDebugger::onThinkEnd);
  HumanoidAgent::think_end_signal.connect(sl);

  redraw_dispatch.connect(sigc::mem_fun(*this, &GtkDebugger::onRedraw));
  dbg_text_dispatch.connect(sigc::mem_fun(*this, &GtkDebugger::onDbgText));
  d_running = true;
  
  //cout << "hello 2!" << endl;
  Gtk::Main::run(*d_window);
}

