#include "gtkdebugger.ih"

void GtkDebugger::onThinkEnd()
{
  if (++d_ticks % 3 == 0)
    redraw_dispatch();
  //d_drawingArea->queue_draw();
  
  /*Glib::RefPtr<Gtk::TreeSelection> refTreeSelection =
    d_objectsList->get_selection();
  
  Gtk::TreeModel::iterator iter = refTreeSelection->get_selected();
  if(iter) //If anything is selected
  {
    Gtk::TreeModel::Row row = *iter;
    Glib::ustring str;
    row.get_value(0, str);
    //cout << str << endl;
    //Do something with the row.
  }*/
}

