#include "gtkdebugger.ih"

void GtkDebugger::start()
{
  //cout << "hello!" << endl;
  Glib::Thread::create(sigc::mem_fun(*this, &GtkDebugger::run), true);
  //pthread_create(&d_thread, 0, startThread, this);
}

