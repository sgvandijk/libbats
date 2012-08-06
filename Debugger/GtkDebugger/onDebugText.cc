#include "gtkdebugger.ih"

void GtkDebugger::onDbgText()
{
  d_dbgTextLock.lock();
  d_dbgText->get_buffer()->insert(d_dbgText->get_buffer()->end(), d_dbg);
  d_dbg = "";
  d_dbgTextLock.unlock();
  Gtk::TextIter end = d_dbgText->get_buffer()->end();
  d_dbgText->scroll_to(end);
}

